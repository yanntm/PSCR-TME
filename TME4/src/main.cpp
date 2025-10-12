// ImageResizerApp.cpp
#include <QCoreApplication>  // For non-GUI Qt app

#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>
#include <utility>
#include <chrono>
#include <cstdlib>
#include <sstream>

#include "util/CLI11.hpp" // Header only lib for argument parsing

#include "util/ImageUtils.h"
#include "BoundedBlockingQueue.h"
#include "Tasks.h"
#include "util/thread_timer.h"
#include "util/processRSS.h"

struct Options {
    std::filesystem::path inputFolder = "input_images/";
    std::filesystem::path outputFolder = "output_images/";
    std::string mode = "resize";
    int num_threads = 4;

  friend std::ostream &operator<<(std::ostream &os, const Options &opts) {
    os << "input folder '" << opts.inputFolder.string() 
       << "', output folder '" << opts.outputFolder.string() 
       << "', mode '" << opts.mode 
       << "', nthreads " << opts.num_threads;
    return os;
  }
};

int parseOptions(int argc, char *argv[], Options& opts);

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);  // Initialize Qt for image format plugins

    Options opts;
    // rely on CLI11 to parse command line options
    // see implementation of parseOptions below main function
    int code = parseOptions(argc, argv, opts);
    if (code != 0) {
        return code;
    }

    std::cout << "Image resizer starting with " << opts << std::endl;

    auto start_time = std::chrono::steady_clock::now();
    pr::thread_timer main_timer;

    if (opts.mode == "resize") {
        // Single-threaded: direct load/resize/save in callback
        pr::findImageFiles(opts.inputFolder, [&](const std::filesystem::path& file) {
            QImage original = pr::loadImage(file);
            if (!original.isNull()) {
                QImage resized = pr::resizeImage(original);
                std::filesystem::path outputFile = opts.outputFolder / file.filename();
                pr::saveImage(resized, outputFile);
            }
        });
    } else if (opts.mode == "pipe") {
        // 1. Single-threaded pipeline: file discovery -> treatImage (load/resize/save)
        pr::FileQueue fileQueue(10);

        // 2. Start the worker thread
        std::thread worker(pr::treatImage, std::ref(fileQueue), std::ref(opts.outputFolder));

        // 3. Populate file queue synchronously
        pr::findImageFiles(opts.inputFolder, [&](const std::filesystem::path& file) {
            fileQueue.push(file);
        });

        // 4. Push poison pill
        fileQueue.push(pr::FILE_POISON);

        // 5. Join the worker thread
        worker.join();
    } else {
        std::cerr << "Unknown mode '" << opts.mode << "'. Supported modes: resize, pipe" << std::endl;
        return 1;
    }

    std::stringstream ss;
    ss << "Thread " << std::this_thread::get_id() << " (main): " << main_timer << " ms CPU" << std::endl;
    std::cout << ss.str();

    auto end = std::chrono::steady_clock::now();
    std::cout << "Total runtime (wall clock): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count() << " ms" << std::endl;

    // Report memory usage at the end
    std::cout << "Memory usage: " << process::getResidentMemory() << std::endl;

    // Report total CPU time across all timers
    std::cout << "Total CPU time across all threads: " << pr::thread_timer::getTotalCpuTimeMs() << " ms" << std::endl;

    return 0;
}

int parseOptions(int argc, char *argv[], Options& opts) {
    Options default_opts;  // Use defaults from struct for CLI11 help
    CLI::App cli_app("Image Resizer Application. Scales down images in input folder, writes to output folder.");

    cli_app.add_option("-i,--input", opts.inputFolder, "Input folder containing images")
        ->check(CLI::ExistingDirectory)
        ->default_str(default_opts.inputFolder.string());

    cli_app.add_option("-o,--output", opts.outputFolder, "Output folder for resized images (will be created if needed)")
        ->default_str(default_opts.outputFolder.string());

    cli_app.add_option("-m,--mode", opts.mode, "Processing mode")
        ->check(CLI::IsMember({"resize", "pipe" /*, "pipe_mt" */})) // TODO : add modes
        ->default_str(default_opts.mode);

    cli_app.add_option("-n,--nthreads", opts.num_threads, "Number of threads")
        ->check(CLI::PositiveNumber)
        ->default_val(default_opts.num_threads);

    try {
        cli_app.parse(argc, argv);
    } catch (const CLI::CallForHelp &e) {
        cli_app.exit(e);
        std::exit(0);
    } catch (const CLI::ParseError &e) {
        return cli_app.exit(e);
    }

    if (!std::filesystem::exists(opts.outputFolder)) {
        if (!std::filesystem::create_directories(opts.outputFolder)) {
            std::cerr << "Failed to create the output folder." << std::endl;
            return 1;
        }
    }

    return 0;
}
