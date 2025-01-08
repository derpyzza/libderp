#pragma once
#include "dtypes.h"

result dalloc   (size datasize);
result dcalloc  (size datasize);
result drealloc (void * data, size datasize);
void   dfree    (void * dataptr);

typedef struct allocator allocator;
struct allocator {
  void * ctx;
  fnptr(void*, dalloc, size datasize);
  fnptr(void, dfree, void * dataptr);
};
