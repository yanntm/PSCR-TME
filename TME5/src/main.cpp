#include "Image.h"
#include "Ray.h"
#include "Renderer.h"
#include "Scene.h"
#include "SceneBuilder.h"
#include "Vec3D.h"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <limits>
#include <vector>

#include "util/CLI11.hpp" // Header only lib for argument parsing

using namespace std;
using namespace pr;

struct Options {
  std::string output = "spheres.bmp";
  int width = 2000;
  int height = 2000;
  int num_spheres = 250;
  std::string mode = "sequential";
  int nbthread = 4;

  friend std::ostream &operator<<(std::ostream &os, const Options &opts) {
    os << "output '" << opts.output << "', resolution " << opts.width << "x" << opts.height
       << ", spheres " << opts.num_spheres << ", mode " << opts.mode;
    if (opts.mode == "ThreadManual" || opts.mode == "PoolPixel" || opts.mode == "PoolRow") {
      os << ", threads " << opts.nbthread;
    }
    return os;
  }
};

int parseOptions(int argc, char *argv[], Options &opts);

int main(int argc, char *argv[]) {
  Options opts;
  // rely on CLI11 to parse command line options
  int code = parseOptions(argc, argv, opts);
  if (code != 0) {
    return code;
  }

  std::cout << "Ray tracer starting with " << opts << std::endl;

  auto start = std::chrono::steady_clock::now();

  // definir la Scene : resolution de l'image
  Scene scene = buildRandomScene(opts.width, opts.height, opts.num_spheres);

  // L'image finale a produire
  Image img(scene.getWidth(), scene.getHeight());

  // Rendre la scène dans l'image
  pr::Renderer renderer;
  if (opts.mode == "sequential") {
    renderer.render(scene, img);
    /*
           } else if (opts.mode == "ThreadPerPixel") {
           renderer.renderThreadPerPixel(scene, img);
       } else if (opts.mode == "ThreadPerRow") {
           renderer.renderThreadPerRow(scene, img);
       } else if (opts.mode == "ThreadManual") {
           renderer.renderThreadManual(scene, img, opts.nbthread);
       } else if (opts.mode == "PoolPixel") {
           renderer.renderPoolPixel(scene, img, opts.nbthread);
       } else if (opts.mode == "PoolRow") {
           renderer.renderPoolRow(scene, img, opts.nbthread);
       } else if (opts.mode == "PoolFunctionalRow") {
           renderer.renderPoolFunctionalRow(scene, img, opts.nbthread);
                   */
  } else {
    std::cerr << "Unknown mode: " << opts.mode << std::endl;
    return 1;
  }

  auto end = std::chrono::steady_clock::now();
  std::cout << "Total time "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << "ms.\n";

  img.exportToBMP(opts.output.c_str());

  return 0;
}

int parseOptions(int argc, char *argv[], Options &opts) {
  Options default_opts; // Use defaults from struct for CLI11 help
  CLI::App cli_app("Ray Tracer Application. Renders a scene with random spheres to a BMP image.");

  cli_app.add_option("-o,--output", opts.output, "Output BMP file name")
      ->default_str(default_opts.output);

  cli_app.add_option("-W,--width", opts.width, "Image width")
      ->check(CLI::PositiveNumber)
      ->default_val(default_opts.width);

  cli_app.add_option("-H,--height", opts.height, "Image height")
      ->check(CLI::PositiveNumber)
      ->default_val(default_opts.height);

  cli_app.add_option("-s,--spheres", opts.num_spheres, "Number of random spheres")
      ->check(CLI::PositiveNumber)
      ->default_val(default_opts.num_spheres);

  cli_app.add_option("-m,--mode", opts.mode, "Processing mode")
      ->check(CLI::IsMember({"sequential", "ThreadPerPixel", "ThreadPerRow", "ThreadManual",
                             "PoolPixel", "PoolRow", "PoolFunctionalRow"}))
      ->default_str(default_opts.mode);

  cli_app.add_option("-n,--nbthread", opts.nbthread, "Number of threads")
      ->check(CLI::PositiveNumber)
      ->default_val(default_opts.nbthread);

  try {
    cli_app.parse(argc, argv);
  } catch (const CLI::CallForHelp &e) {
    cli_app.exit(e);
    std::exit(0);
  } catch (const CLI::ParseError &e) {
    return cli_app.exit(e);
  }

  return 0;
}
