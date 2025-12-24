#include "derp.h"

#include <stdlib.h>
#include <sys/mman.h>

void * default_malloc(isize l, bool clear, d_alloc_data);
void   default_free(void*, isize, d_alloc_data);

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

void default_free(void* ptr, isize len, d_alloc_data data) {
  (void)data;
  (void)len;
  if(ptr) {
    free(ptr);
  }
}


void * d_tracking_alloc(isize l, bool clear, d_alloc_data ctx_data) {
  dassert(l > 0);
  dtrack_alloc_data * data = (dtrack_alloc_data*)ctx_data.ctx;
  void *ptr = malloc(l);
  if(clear) {
    memset(ptr, 0, l);
  }
  data->num_allocations++;
  isize prev = data->memory_allocated;
  data->memory_allocated += l;
    dlog_func(def_logger.level, ctx_data.file, ctx_data.line_num, "[Alloc] made allocation of %zd bytes, prev: %zd, now: %zd", l, prev, data->memory_allocated);
  return ptr;
}

void d_tracking_free(void* ptr, isize len, d_alloc_data ctx_data) {
  if(ptr) {
    dtrack_alloc_data * data = (dtrack_alloc_data*)ctx_data.ctx;
    isize prev = data->memory_freed;
    data->memory_freed += len;
    dlog_func(def_logger.level, ctx_data.file, ctx_data.line_num, "[Alloc] freed %zd bytes, prev: %zd, now: %zd", len, prev, data->memory_allocated);
    free(ptr);
  } else {
    dlog_func(def_logger.level, ctx_data.file, ctx_data.line_num, "tried to free nullptr, size: %zd", len);
  }
}
