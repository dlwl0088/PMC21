/**
 * @file stack.h
 *
 */

#ifndef STACK_H_
#define STACK_H_

#include "common.h"

class Stack {
public:
	explicit Stack(const size_t max);     // Constructor
	~Stack();

	inline size_t size() const;
	inline void clear();
	inline void push(const Vertex it);
	inline Vertex pop();
	inline const Vertex peek() const;

private:
	size_t maxsize;            
	size_t top;                  
	std::vector<Vertex> stack;

};


inline size_t Stack::size() const { return top; }  // Return length

inline void Stack::clear() { top = 0; }           // Reinitialize

inline void Stack::push(const Vertex it) {         // Put "it" on stack
	if (top== maxsize) exit(EXIT_FAILURE);
	stack[top++] = it;
}

inline Vertex Stack::pop() {     	// Pop top element
	if (top == 0) exit(EXIT_FAILURE);
	return stack[--top];
}

inline const Vertex Stack::peek() const {     // Return top element
	if (top == 0) exit(EXIT_FAILURE);
	return stack[top - 1];
}

#endif  // STACK_H_