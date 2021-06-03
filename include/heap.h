/**
 * @file heap.h
 *
 */

#ifndef HEAP_H_
#define HEAP_H_

#include "common.h"

class Heap {
public:
	explicit  Heap(const size_t max, std::vector<size_t> m);     // Constructor
	~Heap();

	inline size_t size() const;
	inline bool isLeaf(size_t pos) const;
	inline size_t leftchild(size_t pos) const;
	inline size_t rightchild(size_t pos) const;
	inline size_t parent(size_t pos) const;
	inline void buildHeap();
	inline void swap(size_t a, size_t b);
	inline void insert(Vertex it);
	inline Vertex remove();
	inline Vertex remove(size_t pos);
	inline Vertex remove(Vertex it);
	inline Vertex peek();

private:
	std::vector<Vertex> heap;          // Pointer to the heap array
	size_t maxsize;         // Maximum size of the heap
	size_t n;               // Number of elements now in the heap
	std::vector<size_t> measure;
	std::vector<size_t> idx;

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
	if (n < 2) return;
	size_t i = (n / 2) - 1;
	while (true) {
		siftdown(i);
		if (i == 0) break;
		i--;
	}
}

inline void Heap::swap(size_t a, size_t b) {
	idx[heap[a]] = b;
	idx[heap[b]] = a;

	size_t temp = heap[a];
	heap[a] = heap[b];
	heap[b] = temp;

}

// Insert "it" into the heap
inline void Heap::insert(const Vertex it) {
	if (n >= maxsize) exit(EXIT_FAILURE);

	size_t curr = n++;
	heap[curr] = it;            // Start at end of heap
	idx[it] = curr;
	// Now sift up until curr's parent < curr
	while ((curr != 0) &&
		(measure[heap[curr]] > measure[heap[parent(curr)]])) {
		swap(curr, parent(curr));
		curr = parent(curr);
	}
}


inline void Heap::siftdown(size_t pos) {
	while (!isLeaf(pos)) { // Stop if pos is a leaf
		size_t j = leftchild(pos);
		size_t RightChild = rightchild(pos);
		if ((RightChild < n) && (measure[heap[RightChild]] > measure[heap[j]]))	j = RightChild;  // Set j to smaller child's value
		if (heap[pos] > heap[j]) return; // Done
		swap(pos, j);
		pos = j;             // Move down
	}
}

// Remove first value
inline Vertex Heap::remove() {
	if (n == 0) exit(EXIT_FAILURE);
	swap(0, --n);       // Swap first with last value
	if (n != 0) siftdown(0);  // Siftdown new root val
	return heap[n];             // Return deleted value
}

inline Vertex Heap::peek() {
	if (n == 0) exit(EXIT_FAILURE);
	return heap[0];
}


// Remove and return element at specified position
inline Vertex  Heap::remove(size_t pos) {
	if (pos >= n) exit(EXIT_FAILURE);
	if (pos == (n - 1)) n--; // Last element, no work to do
	else
	{
		swap(pos, --n);          // Swap with last value
		while ((pos != 0) && (measure[heap[pos]] > measure[heap[parent(pos)]])) {
			swap(pos, parent(pos)); // Push up large key
			pos = parent(pos);
		}
		if (n != 0) siftdown(pos);     // Push down small key
	}
	return heap[n];
}

inline Vertex Heap::remove(Vertex it) {
	return remove(idx[it]);
}

#endif  // HEAP_H_