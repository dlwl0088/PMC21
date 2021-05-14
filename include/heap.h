/**
 * @file heap.h
 *
 */

#ifndef HEAP_H_
#define HEAP_H_

#include <assert.h>


#include "common.h"

class Heap {
public:
	explicit  Heap(const size_t max, size_t* m);     // Constructor
	~Heap();

	inline size_t size() const;
	inline bool isLeaf(size_t pos) const;
	inline size_t leftchild(size_t pos) const;
	inline size_t rightchild(size_t pos) const;
	inline size_t parent(size_t pos) const;
	inline void buildHeap();
	inline void swap(size_t a, size_t b);
	inline void insert(const size_t& it);
	inline size_t remove();

private:
	Vertex* heap;          // Pointer to the heap array
	size_t maxsize;         // Maximum size of the heap
	size_t n;               // Number of elements now in the heap
	size_t* measure;

	inline void siftdown(size_t pos);
	// Helper function to put element in its correct place


};

inline size_t Heap::size() const       // Return current heap size
{
	return n;
}
inline bool Heap::isLeaf(size_t pos) const // True if pos is a leaf
{
	return (pos >= n / 2) && (pos < n);
}
inline size_t Heap::leftchild(size_t pos) const
{
	return 2 * pos + 1;
}    // Return leftchild position
inline size_t Heap::rightchild(size_t pos) const
{
	return 2 * pos + 2;
}    // Return rightchild position
inline size_t Heap::parent(size_t pos) const  // Return parent position
{
	return (pos - 1) / 2;
}
inline void Heap::buildHeap()           // Heapify contents of Heap
{
	for (size_t i = n / 2 - 1; i >= 0; i--) siftdown(i);
}
inline void Heap::swap(size_t a, size_t b) { size_t temp = heap[a]; heap[a] = heap[b]; heap[b] = temp; }

// Insert "it" into the heap
inline void Heap::insert(const size_t& it) {
	assert(n < maxsize, "Heap is full");
	size_t curr = n++;
	heap[curr] = it;            // Start at end of heap
	// Now sift up until curr's parent < curr
	while ((curr != 0) &&
		(measure[heap[curr]] < measure[heap[parent(curr)]])) {
		swap(curr, parent(curr));
		curr = parent(curr);
	}
}
// Remove first value
inline size_t Heap::remove() {
	assert(n > 0, "Heap is empty");
	swap(0, --n);       // Swap first with last value
	if (n != 0) siftdown(0);  // Siftdown new root val
	return heap[n];             // Return deleted value
}


inline void Heap::siftdown(size_t pos) {
	while (!isLeaf(pos)) { // Stop if pos is a leaf
		size_t j = leftchild(pos);  size_t rc = rightchild(pos);
		if ((rc < n) && (measure[heap[rc]] < measure[heap[j]]))
			j = rc;            // Set j to smaller child's value
		if (heap[pos] < heap[j]) return; // Done
		swap(pos, j);
		pos = j;             // Move down
	}
}

#endif  // HEAP_H_