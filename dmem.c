#include "derp.h"

#include <stdlib.h>
#include <sys/mman.h>

void * default_malloc(isize l, bool clear, d_alloc_data);
void * default_realloc(void* data, isize l, d_alloc_data);
void   default_free(void*, d_alloc_data);

d_allocator def_allocator = {
  .alloc = default_malloc,
  .realloc = default_realloc,
  .free = default_free,
  0
};

void dalloc_set_default(d_allocator alloc) {
  def_allocator = alloc;
}

void * default_malloc(isize l, bool clear, d_alloc_data data) {
  (void)data;

  dassert(l > 0, "malloc size must not be zero");

  void *ptr = malloc(l);
  dassert(ptr != NULL, "malloc returned null pointer :(")

  if(clear) {
    memset(ptr, 0, l);
  }
  return ptr;
}

void * default_realloc(void* block, isize l, d_alloc_data data) {
  (void)data;

  dassert(l > 0, "realloc size must not be zero");

  void *ptr = realloc(block, l);
  dassert(ptr != NULL, "realloc returned null pointer :(")

  return ptr;
}

void default_free(void* ptr, d_alloc_data data) {
  (void)data;
  free(ptr);
}
