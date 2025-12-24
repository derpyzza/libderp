#include "derp.h"

#include <stdlib.h>
#include <sys/mman.h>

void * default_malloc(isize l, bool clear, void*);
void   default_free(void*, isize, void*);

d_allocator def_allocator = {
  .alloc = default_malloc,
  .free = default_free,
  0
};

void dalloc_set_default(d_allocator alloc) {
  def_allocator = alloc;
}

void * default_malloc(isize l, bool clear, void* ctx) {
  (void)ctx;
  dassert(l > 0);
  void *ptr = malloc(l);
  if(clear) {
    memset(ptr, 0, l);
  }
  return ptr;
}

void default_free(void* ptr, isize len, void* ctx) {
  (void)ctx;
  (void)len;
  if(ptr) {
    free(ptr);
  }
}


void * d_tracking_alloc(isize l, bool clear, void* ctx) {
  dassert(l > 0);
  dtrack_alloc_data * data = (dtrack_alloc_data*)ctx;
  void *ptr = malloc(l);
  if(clear) {
    memset(ptr, 0, l);
  }
  data->num_allocations++;
  data->memory_taken += l;
  return ptr;
}

void d_tracking_free(void* ptr, isize len, void* ctx) {
  if(ptr) {
    dtrack_alloc_data * data = (dtrack_alloc_data*)ctx;
    data->memory_taken -= len;
    free(ptr);
  }
}
