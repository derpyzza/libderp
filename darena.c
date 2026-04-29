#include "derp.h"

d_bucket * dbucket_new(isize size) {
  isize sb = sizeof(d_bucket) + sizeof(isize) * size;

  d_bucket * b = d_alloc(sb);
  b->next = NULL;
  b->size = size;
  b->index = 0;
  return b;
}

void darena_init(d_arena * buf, isize size) {
  d_bucket * buck = dbucket_new(size);
  buf->head = buck;
  buf->tail = buck;
}

void * darena_alloc (d_arena *buf, isize size) {
  size = d_align(size, sizeof(isize) - 1);

  if(buf->tail == NULL) {
    buf->tail = dbucket_new(size);
    buf->head = buf->tail;
  }

  while(buf->tail->index + size > buf->tail->size && buf->tail->next != NULL ) {
    buf->tail = buf->tail->next;
  }

  if (buf->tail->index + size > buf->tail->size) {
    buf->tail->next = dbucket_new(size);
    buf->tail = buf->tail->next;
  }

  void *result = &buf->tail->data[buf->tail->index];
  buf->tail->index += size;
  return result;

  buf->allocations++;
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
    d_bucket * bb = b;
    b = b->next;
    d_free(bb);
  }

  buf->head = NULL;
  buf->tail = NULL;
}
