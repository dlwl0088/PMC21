/**
 * @file stack.cc
 *
 */

#include "stack.h"

Stack::Stack(const size_t max) {
    maxsize = max;
    stack.resize(max);
    top = 0;
}

Stack::~Stack() {
}