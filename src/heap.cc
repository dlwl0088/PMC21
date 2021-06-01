/**
 * @file heap.cc
 *
 */

#include "heap.h"

Heap::Heap(const size_t max, std::vector<size_t> m) {
    heap.resize(max);
    measure = m;
    n = 0;
    maxsize = max;
    Heap::buildHeap();
}

Heap::~Heap() {
}