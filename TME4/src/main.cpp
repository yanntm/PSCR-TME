// Réduction d’images : variantes séquentielle et concurrentes.
#include <QCoreApplication>  // Application sans interface graphique

#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>
#include <utility>
#include <chrono>
#include <cstdlib>
#include <sstream>

#include "util/CLI11.hpp" // Options de la ligne de commande

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
    int nbread = 1;
    int nbresize = 1;
    int nbwrite = 1;
    int queue_size = 10;

  friend std::ostream &operator<<(std::ostream &os, const Options &opts) {
    os << "input folder '" << opts.inputFolder.string() 
       << "', output folder '" << opts.outputFolder.string() 
       << "', mode '" << opts.mode 
       << "', nthreads " << opts.num_threads
       << ", nbread " << opts.nbread
       << ", nbresize " << opts.nbresize
       << ", nbwrite " << opts.nbwrite
       << ", queue_size " << opts.queue_size;
    return os;
  }
};

int parseOptions(int argc, char *argv[], Options& opts);

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);  // Charger les plugins de formats d’image

    Options opts;
    // Le parsing des options est fourni.
    // Voir parseOptions sous le main.
    int code = parseOptions(argc, argv, opts);
    if (code != 0) {
        return code;
    }

    std::cout << "Image resizer starting with " << opts << std::endl;

    auto start_time = std::chrono::steady_clock::now();
    pr::thread_timer main_timer;

    if (opts.mode == "resize") {
        // Traitement séquentiel complet de chaque image.
        pr::findImageFiles(opts.inputFolder, [&](const std::filesystem::path& file) {
            QImage original = pr::loadImage(file);
            if (!original.isNull()) {
                QImage resized = pr::resizeImage(original);
                std::filesystem::path outputFile = opts.outputFolder / file.filename();
                pr::saveImage(resized, outputFile);
            }
        });
    } else if (opts.mode == "pipe") {
        // 1. Construire la file de chemins.
        pr::FileQueue fileQueue(opts.queue_size);

        // 2. Lancer le consommateur.
        std::thread worker(pr::treatImage, std::ref(fileQueue), std::ref(opts.outputFolder));

        // 3. Insérer les chemins découverts.
        pr::findImageFiles(opts.inputFolder, [&](const std::filesystem::path& file) {
            fileQueue.push(file);
        });

        // 4. Annoncer la fin des chemins.
        fileQueue.push(pr::FILE_POISON);

        // 5. Attendre la fin du consommateur.
        worker.join();
    } else if (opts.mode == "pipe_mt") {
        // Q3 : partir d’une copie du mode pipe.
        std::cerr << "Mode pipe_mt a implementer (Q3).\n";
        return 2;
    } else if (opts.mode == "mt_pipeline") {
        // Q5 puis Q6 : connecter les étapes, puis paramétrer leurs effectifs.
        std::cerr << "Mode mt_pipeline a implementer (Q5).\n";
        return 2;
    } else {
        std::cerr << "Unknown mode '" << opts.mode << "'. Supported modes: resize, pipe, pipe_mt, mt_pipeline" << std::endl;
        return 1;
    }

    std::stringstream ss;
    ss << "Thread " << std::this_thread::get_id() << " (main): " << main_timer << " ms CPU" << std::endl;
    std::cout << ss.str();

    auto end = std::chrono::steady_clock::now();
    std::cout << "Total runtime (wall clock): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count() << " ms" << std::endl;

    // Mesures du processus.
    auto rss = process::getResidentMemory();
    std::cout << "Memory usage: " << rss << std::endl;
    std::cout << "Peak RSS (bytes): " << rss.peak << std::endl;

    // Somme des temps CPU mesurés.
    std::cout << "Total CPU time across all threads: " << pr::thread_timer::getTotalCpuTimeMs() << " ms" << std::endl;

    return 0;
}

int parseOptions(int argc, char *argv[], Options& opts) {
    Options default_opts;  // Valeurs affichées par --help
    CLI::App cli_app("Image Resizer Application. Scales down images in input folder, writes to output folder.");

    cli_app.add_option("-i,--input", opts.inputFolder, "Input folder containing images")
        ->check(CLI::ExistingDirectory)
        ->default_str(default_opts.inputFolder.string());

    cli_app.add_option("-o,--output", opts.outputFolder, "Output folder for resized images (will be created if needed)")
        ->default_str(default_opts.outputFolder.string());

    cli_app.add_option("-m,--mode", opts.mode, "Processing mode")
        ->check(CLI::IsMember({"resize", "pipe", "pipe_mt", "mt_pipeline"}))
        ->default_str(default_opts.mode);

    cli_app.add_option("-n,--nthreads", opts.num_threads, "Number of threads")
        ->check(CLI::PositiveNumber)
        ->default_val(default_opts.num_threads);

    cli_app.add_option("-r,--nbread", opts.nbread, "Number of reader threads")
        ->check(CLI::PositiveNumber)
        ->default_val(default_opts.nbread);

    cli_app.add_option("-s,--nbresize", opts.nbresize, "Number of resizer threads")
        ->check(CLI::PositiveNumber)
        ->default_val(default_opts.nbresize);

    cli_app.add_option("-w,--nbwrite", opts.nbwrite, "Number of writer threads")
        ->check(CLI::PositiveNumber)
        ->default_val(default_opts.nbwrite);

    cli_app.add_option("-q,--queue-size", opts.queue_size, "Capacite des files")
        ->check(CLI::PositiveNumber)->default_val(opts.queue_size);

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
