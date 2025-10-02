#pragma once

#include <atomic>
#include <memory>
#include <new>
#include <string>
#include <utility>
#include <vector>

// version improved with @deduplicator on codereview.stackexchange, and others

// Lock-free linked list for word frequency counting
// This implementation uses tail insertion to maintain proper ordering
// and avoid the complexity of duplicate merging
class alignas(std::hardware_destructive_interference_size) ListLF {

  // alignas at declaration to avoid "false sharing" because our lists (sizeof is only one atomic pointer head_) are
  // packed closely in the "buckets" vector of hash map and would otherwise share cache lines

private:
  // Node structure for linked list
  // not aligned, debatable, but typically not packed closely
  // nor under so much contention as list heads.
  struct Node {
    std::string key;
    std::atomic<int> count;
    std::atomic<Node *> next;

    Node(const std::string &k, int initial_count = 1) : key(k), count(initial_count), next(nullptr) {}
  };
  // Head pointer of the list, atomic for lock-free updates
  std::atomic<Node *> head_;

public:
  ListLF() : head_(nullptr) {}

  ~ListLF() {
    Node *current = head_.load();
    while (current) {
      Node *next = current->next.load();
      delete current;
      current = next;
    }
  }

  // Increment count for the given word, or insert with count=delta if not found
  // Uses lock-free algorithm to handle concurrent insertions safely
  // This version uses tail insertion to maintain order and avoid duplicates
  void incrementCount(const std::string &word, int delta = 1) {
    Node *new_node = nullptr; // Allocate outside loop for reuse optimization
    // position is initially set to &head, then as we iterate to &next in successive nodes
    // it points to the tentative node->next/head_ we might overwrite if we insert, can only progress
    std::atomic<Node *> *position = &head_;
    // current : a node pointer equal to *position, order_acquire so any previous write is visible
    Node *current = position->load(std::memory_order_acquire);
    do {
      // STEP 1: TRAVERSAL - Walk through the list from current position
      // We use memory_order_acquire to ensure we see all previous writes
      while (current) {
        if (current->key == word) {
          // FOUND: Word exists - atomically increment its count
          // fetch_add returns the previous value, so we add delta to increment
          // Use relaxed ordering since other threads don't depend on this order
          current->count.fetch_add(delta, std::memory_order_relaxed);
          delete new_node; // Clean up if we allocated but were overtaken by a
                           // same word thread ; else NOOP
          return;
        }
        position = &current->next;
        current = position->load(std::memory_order_acquire);
      }
      // STEP 2: WORD NOT FOUND - Prepare for tail insertion
      // Invariant : current==nullptr
      // ie. our last read of *position was nullptr
      if (!new_node) {
        new_node = new Node(word, delta);
      }
      // CRITICAL ATOMIC OPERATION: compare_exchange_weak
      // This is the "test-and-set" that makes this lock-free
      // - If *position is still nullptr as expected, CAS succeeds and sets *position = new_node, we
      // successfully appended
      // - Else (we were overtaken), CAS fails and updates current with the new tail
      // order_acq_rel ensures this operation has proper memory ordering
    } while (!position->compare_exchange_weak(current, new_node, std::memory_order_acq_rel, std::memory_order_acquire));
    // SUCCESS: Our node is now appended to the list, we are done
    // FAILURE: Another thread modified *position, continue from new loaded current
  }

  // Convert to vector of pairs for result collection
  // This method is called single-threaded after all insertions complete
  // Non-thread-safe for simplicity and performance
  std::vector<std::pair<std::string, int>> toKeyValuePairs() const {
    std::vector<std::pair<std::string, int>> result;
    Node *current = head_; // Simple load since single-threaded
    while (current) {
      result.emplace_back(current->key, current->count);
      current = current->next;
    }
    return result;
  }
};
