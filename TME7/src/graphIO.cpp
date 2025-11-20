// main.cpp
#include "BinIO.h"
#include "EdgeListIO.h"

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " [c|d] input output" << std::endl;
    return 1;
  }

  std::string mode = argv[1];
  if (mode == "c") {
    Graph graph = parseEdgeList(argv[2]);
    writeBin(graph, argv[3]);
  } else if (mode == "d") {
    Graph graph = parseBin(argv[2]);
    writeEdgeList(graph, argv[3]);
  } else {
    std::cerr << "Invalid mode: " << mode << std::endl;
    return 1;
  }

  return 0;
}