// EdgeListIO.h
#pragma once

#include "Graph.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Graph parseEdgeList(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open " << filename << std::endl;
    exit(1);
  }

  Graph graph;
  size_t src, dest;
  while (file >> src >> dest) {
    graph.addEdge(src, dest);
  }

  file.close();

  return graph;
}

void writeEdgeList(const Graph &graph, const std::string &filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open " << filename << std::endl;
    exit(1);
  }

  for (size_t i = 0; i < graph.size(); ++i) {
    auto source = graph.findNode(i);
    for (Vertex *child : source->children) {
      file << source->id << " " << child->id << "\n";
    }
  }

  file.close();
}