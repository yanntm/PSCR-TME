// Graph.h
#pragma once

#include <cstdint>
#include <vector>

struct Vertex {
  size_t id;
  std::vector<Vertex *> children;
};

class Graph {
  std::vector<Vertex *> vertices;

public:
  Graph(size_t N = 1) : vertices() {
    vertices.reserve(N);
    findNode(N - 1); // Ensure at least N nodes
  }
  ~Graph() {
    for (auto v : vertices)
      delete v;
  }

  void addEdge(size_t src, size_t dest) { findNode(src)->children.push_back(findNode(dest)); }

  Vertex *findNode(size_t id) {
    if (id >= vertices.size()) {
      for (size_t i = vertices.size(); i <= id; ++i) {
        vertices.push_back(new Vertex{i, {}});
      }
    }
    return vertices[id];
  }
  const Vertex *findNode(size_t id) const {
    return (id < vertices.size()) ? vertices[id] : nullptr;
  }

  size_t size() const { return vertices.size(); }
};