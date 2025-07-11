#include "derp.h"

void darena_init(d_arena * buf, isize size) {
  buf->size = size;
  buf->allocated = 0;
  buf->buf = 0;
}

// initializes and allocates memory for an arena object
d_arena darena_init_alloc (isize size) {
  d_arena arr = {0};
  darena_init(&arr, size);
  arr.buf = d_alloc(sizeof(size));
  if(!arr.buf) { dlog_error("couldnt allocate new arena"); }
  return arr;
}

// allocates memory given an arena
void * darena_alloc (d_arena *buf, isize len) {
  void * t = buf->buf + buf->allocated;
  buf->allocated += len;
  buf->allocations++;
  return t;
}
void darena_free (d_arena* buf) {
  buf->allocated = 0;
  buf->allocations = 0;
}
