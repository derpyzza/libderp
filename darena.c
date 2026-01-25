#include "derp.h"

void darena_init(d_arena * buf, isize size) {
  buf->capacity = size;
  buf->size = 0;
  buf->buf = d_alloc(size);
}

void * darena_alloc (d_arena *buf, isize size) {
  size = d_align(size, 7);

  if(buf->size + size > buf->capacity) {
    isize new_cap = buf->size * 2;

    if(new_cap < buf->size + size) {
      new_cap = buf->size + size;
    }

    u8 * data = d_realloc(buf->buf, new_cap);
    if (!data) {
      return NULL;
    }

    buf->buf = data;
    buf->capacity = new_cap;
  }
  
  void *ptr = buf->buf + buf->size;
  buf->size += size;
  buf->allocations++;
  return ptr;
}

void darena_clear ( d_arena* buf ) {
  buf->size = 0;
  buf->allocations = 0;
}

void darena_free (d_arena* buf) {
  buf->size = 0;
  buf->allocations = 0;
  d_free(buf->buf);
}
