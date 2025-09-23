#pragma once

#include <string>
#include <atomic>
#include <memory>
#include <vector>
#include <utility>

// Lock-free linked list for word frequency counting
// This implementation uses tail insertion to maintain proper ordering
// and avoid the complexity of duplicate merging
class ListLF {
private:
    struct Node {
        std::string key;
        std::atomic<int> count;
        std::atomic<Node*> next;

        Node(const std::string& k, int initial_count = 1)
            : key(k), count(initial_count), next(nullptr) {}
    };

    std::atomic<Node*> head_;

public:
    ListLF() : head_(nullptr) {}

    ~ListLF() {
        Node* current = head_.load();
        while (current) {
            Node* next = current->next.load();
            delete current;
            current = next;
        }
    }

    // Increment count for the given word, or insert with count=delta if not found
    // Uses lock-free algorithm to handle concurrent insertions safely
    // This version uses tail insertion to maintain order and avoid duplicates
    void incrementCount(const std::string& word, int delta = 1) {
        Node* new_node = nullptr;  // Allocate outside loop for reuse optimization

        // OPTIMIZATION: Start traversal from head, but allow continuation from better positions
        Node* current = head_.load(std::memory_order_acquire);

        while (true) {
            // STEP 1: TRAVERSAL - Walk through the list from current position
            // We use memory_order_acquire to ensure we see all previous writes
            Node* prev = nullptr;

            while (current) {
                if (current->key == word) {
                    // FOUND: Word exists - atomically increment its count
                    // fetch_add returns the previous value, so we add delta to increment
                    // Use relaxed ordering since other threads don't depend on this order
                    current->count.fetch_add(delta, std::memory_order_relaxed);
                    if (new_node) delete new_node;  // Clean up if we allocated but found existing
                    return;
                }
                prev = current;
                // Continue traversal with acquire semantics
                current = current->next.load(std::memory_order_acquire);
            }

            // STEP 2: WORD NOT FOUND - Prepare for tail insertion
            if (!new_node) {
                new_node = new Node(word, delta);
            }

            if (prev == nullptr) {
                // SPECIAL CASE: List is empty or we started from a modified head
                // This requires special handling because head_ might be modified concurrently

                // CRITICAL ATOMIC OPERATION: compare_exchange_weak
                // This is the "test-and-set" that makes this lock-free
                // - If head_ == current (expected), CAS succeeds and sets head_ = new_node
                // - If head_ != current, CAS fails and updates current with current head_
                // - memory_order_acq_rel ensures this operation has proper memory ordering
                Node* expected = current;  // current should be head value
                if (head_.compare_exchange_weak(expected, new_node,
                                               std::memory_order_acq_rel,
                                               std::memory_order_acquire)) {
                    // SUCCESS: Our node is now the head of the list
                    return;
                } else {
                    // FAILURE: Head was modified, continue from the new head value
                    current = expected;  // expected now contains the new head value
                    continue;  // Continue outer loop with updated current
                }
            } else {
                // NORMAL CASE: Insert at tail after 'prev' node
                // This is more complex because prev->next might be modified concurrently

                // First, read what prev currently points to (should be nullptr for tail)
                Node* expected_next = prev->next.load(std::memory_order_acquire);

                // OPTIMIZATION: If expected_next is not nullptr, another thread inserted
                // Continue traversal from the new successor we discovered
                if (expected_next != nullptr) {
                    current = expected_next;
                    continue;  // Continue outer loop from discovered position
                }

                // CRITICAL ATOMIC OPERATION: Try to link new_node after prev
                // This compare_exchange_weak is the key to lock-free insertion:
                // - If prev->next == nullptr (expected), CAS succeeds and sets prev->next = new_node
                // - If prev->next != nullptr, CAS fails and updates expected_next
                // - memory_order_acq_rel ensures proper ordering for this operation
                if (prev->next.compare_exchange_weak(expected_next, new_node,
                                                    std::memory_order_acq_rel,
                                                    std::memory_order_acquire)) {
                    // SUCCESS: Our node is now properly linked as the new tail
                    return;
                } else {
                    // FAILURE: Another thread modified prev->next
                    // Since we already verified expected_next was nullptr before CAS,
                    // after failure it must now be non-nullptr (another thread inserted)
                    current = expected_next;
                    continue;  // Continue outer loop from discovered position
                }
            }
        }
    }

    // Convert to vector of pairs for result collection
    // This method is called single-threaded after all insertions complete
    // Non-thread-safe for simplicity and performance
    std::vector<std::pair<std::string, int>> toKeyValuePairs() const {
        std::vector<std::pair<std::string, int>> result;
        Node* current = head_.load();  // Simple load since single-threaded
        while (current) {
            result.emplace_back(current->key, current->count.load());  // Simple load
            current = current->next.load();  // Simple load
        }
        return result;
    }
};