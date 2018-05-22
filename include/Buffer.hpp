#ifndef BUFFER_HPP_
#define BUFFER_HPP_
#include <cstdlib>

struct Buffer {
  char *base;
  size_t len;

  Buffer(char *tBase, size_t tLen) : base(tBase), len(tLen) {}
};

#endif