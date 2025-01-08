#pragma once
#include "ddebug.h"

typedef enum derr {
  D_MEM_NEG_ALLOC,
  D_MEM_OUT_OF_MEMORY,
  D_FILE_READ_ERROR,
  D_ERR_MAX = 255
} derr ;

static inline void derror(derr type) {
  switch (type) {
    case D_MEM_NEG_ALLOC: dlog_error("Tried allocating with a negative index"); break;
    case D_MEM_OUT_OF_MEMORY: dlog_error("Out of Memory, couldn't allocate more"); break;
    case D_FILE_READ_ERROR: dlog_error("Couldn't read file");
    default: break;
  }
}
