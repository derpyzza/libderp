#include "derp.h"

#include <stdlib.h>
#include <sys/mman.h>

void * default_malloc(isize l, bool clear, d_alloc_data);
void   default_free(void*, d_alloc_data);

d_allocator def_allocator = {
  .alloc = default_malloc,
  .free = default_free,
  0
};

void dalloc_set_default(d_allocator alloc) {
  def_allocator = alloc;
}

void * default_malloc(isize l, bool clear, d_alloc_data data) {
  (void)data;
  dassert(l > 0);
  void *ptr = malloc(l);
  if(clear) {
    memset(ptr, 0, l);
  }
  return ptr;
}

void default_free(void* ptr, d_alloc_data data) {
  (void)data;
  free(ptr);
}


void * d_tracking_alloc(isize l, bool clear, d_alloc_data ctx_data) {
  dassert(l > 0);
  dtrack_alloc_data * data = (dtrack_alloc_data*)ctx_data.ctx;
  void *ptr = malloc(l);
  if(clear) {
    memset(ptr, 0, l);
  }
  data->num_allocations++;
  data->memory_allocated += l;
  return ptr;
}

void d_tracking_free(void* ptr, d_alloc_data ctx_data) {
  (void) ctx_data;
  free(ptr);
}
