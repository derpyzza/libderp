#include "derp.h"

#include <stdlib.h>
#include <sys/mman.h>

d_allocator def_allocator = {
  default_malloc,
  default_free,
  0
};

void * default_malloc(isize l, void* ctx) {
  (void)ctx;
  dassert(l > 0)
  return malloc(l);
}

void default_free(void* ptr, void* ctx) {
  (void)ctx;
  if(ptr) {
    free(ptr);
  }
}

void mmap_init(d_vmem * mem, isize len) {
  void * ptr = 0;
  #if PLATFORM_WINDOWS

  #elif PLATFORM_POSIX
    size min_size = len / 16;
    if(min_size < 1) min_size = len;
    while(len >= min_size) {
      ptr = mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
      // success?
      if(ptr != MAP_FAILED && ptr) break;
      // did it fail in a non-retriable way?
      		if (errno != ENOMEM && errno != EOVERFLOW && errno != EAGAIN) break;
  		// Try a smaller size
  		len /= 2;  
    }

    if((ptr == MAP_FAILED) || !ptr) { /* ERROR */ }
  #else
    // PANIC
  #endif
  mem->size = len;
  mem->buf = ptr;
  mem->allocated = 0;
}

void * mmap_alloc(d_vmem* mem, isize alloc) {
  isize alloc_after = alloc + mem->allocated;

  #if PLATFORM_WINDOWS

  #endif

  void* ptr = ((u8*)mem->buf) + mem->allocated;
  mem->allocated = alloc_after;
	if (mem->size < alloc_after) {
		dlog_error("Error: The compiler ran out of memory! Over %u MB was allocated from a single memory mem, perhaps you called some recursive macro?", (unsigned)(mem->size / (1024 * 1204)));
		//error exit
	}
	return ptr;
}

isize max = 0x10000000;

void dvmem_init(d_vmem * buf, isize mb_size) {
  if(mb_size > max) mb_size = max;
  mmap_init(buf, 1024 * 1024 * mb_size);
}

void* dvmem_alloc(d_vmem* mem, isize size) {
  return mmap_alloc(mem, size);
}

void darena_init(d_arena * buf, isize size) {
  buf->size = size;
  buf->allocated = 0;
  buf->buf = 0;
}
