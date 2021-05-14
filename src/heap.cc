/**
 * @file heap.cc
 *
 */

#include "heap.h"

Heap::Heap(const size_t max, size_t* m) {
    heap = new Vertex[max]();
    measure = m;
    n = 0;
    maxsize = max;
    Heap::buildHeap();
}

Heap::~Heap() {
    delete[] heap;
}