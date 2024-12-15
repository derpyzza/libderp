// #include <stdio.h>
#include <stdlib.h>
#include "../include/derror.h"
#include "../include/dmem.h"
#include "../include/ddebug.h"

result dalloc(size datasize) {
  result r = { 0 };
  if (datasize < 0) {
    dlog_error("tried allocating a negative sized chunk of memory");
    r.err = D_MEM_NEG_ALLOC;
    return r;
  }

  void * mem = malloc(datasize);
  if (!mem) {
    dlog_error("Couldn't allocate memory :(");
    r.err = D_MEM_OUT_OF_MEMORY;
    return r;
  } else {
    r.ok = mem;
    return r;
  }
}

result drealloc(void * ptr, size datasize) {
  result r = { 0 };
  if (datasize < 0) {
    dlog_error("tried allocating a negative sized chunk of memory");
    r.err = D_MEM_NEG_ALLOC;
    return r;
  }

  void * mem = realloc(ptr, datasize);
  if (!mem) {
    dlog_error("Couldn't allocate memory :(");
    r.err = D_MEM_OUT_OF_MEMORY;
    return r;
  } else {
    r.ok = mem;
    return r;
  }
}

void dfree(void * ptr) {
  if (!ptr) {
    dlog_warn("Attempted to free a null pointer, please remove that");
    return;
  }
  free(ptr);
}
