#include "derp.h"
#include <stddef.h>


void* darena_allocator_alloc(isize len, bool clean, d_alloc_data data) {
  d_arena * arena = (d_arena*)data.ctx;
  void * out = darena_alloc(arena, len);
  if (out && clean) memset(out, 0, len);
  return out;
}

void* darena_allocator_realloc(void * block, isize len, d_alloc_data data) {
  (void) block;
  (void) len;
  (void) data;
  dlog_error("Arena reallocation not implemented yet");
  return NULL;
}

void darena_allocator_free(void* ptr, d_alloc_data data) {
  (void)ptr;
  (void)data;
}

d_allocator darena_make_allocator( d_arena * arena ) {
  return (d_allocator) {
    darena_allocator_alloc,
    darena_allocator_realloc,
    darena_allocator_free,
    (void*)arena
  };
}

d_bucket * dbucket_new(isize size, d_allocator alloc) {
  isize sb = sizeof(d_bucket) + sizeof(isize) * size;

  d_bucket * b = dalloc_alloc(alloc, sb);
  if(!b) { return NULL; }
  b->next = NULL;
  b->size = size;
  b->index = 0;
  return b;
}

d_arena darena_init(isize size, d_allocator alloc) {
  d_arena out = { 0 };
  out.alloc = alloc;
  out.bucket_size = size;
  d_bucket * buck = dbucket_new(size, out.alloc);
  out.head = buck;
  out.tail = buck;
  out.allocations = 0;
  return out;
}

void * darena_alloc (d_arena *buf, isize size) {
  if(size < 0) return NULL;
  isize _size = (size + sizeof(size) - 1) / sizeof(isize);

  if(buf->tail == NULL) {
    buf->tail = dbucket_new(MAX(buf->bucket_size, _size), buf->alloc);
    if(!buf->tail) return NULL;
  }

  while(buf->tail->index + _size > buf->tail->size && buf->tail->next != NULL ) {
    buf->tail = buf->tail->next;
  }

  if (buf->tail->index + _size > buf->tail->size) {
    d_bucket * nb = dbucket_new(MAX(buf->bucket_size, _size), buf->alloc);
    if(!nb) return NULL;
    buf->tail->next = nb;
    buf->tail = nb;
  }

  void *result = &buf->tail->data[buf->tail->index];
  buf->tail->index += _size;
  buf->allocations++;

  return result;
}

void darena_clear ( d_arena* buf ) {
  for (d_bucket * b = buf->head; b != NULL; b = b->next ) {
    b->index = 0;
  }

  buf->tail = buf->head;
}

void darena_free (d_arena* buf) {
  d_bucket * b = buf->head;
  while ( b ) {
    d_bucket * next = b->next;
    dalloc_free(buf->alloc, b);
    b = next;
  }

  buf->head = NULL;
  buf->tail = NULL;
}
