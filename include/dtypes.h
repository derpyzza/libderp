#pragma once

#include <stdint.h>

typedef int8_t       i8;
typedef int16_t      i16;
typedef int32_t      i32;
typedef int64_t      i64;
typedef uint8_t      u8;
typedef uint16_t     u16;
typedef uint32_t     u32;
typedef uint64_t     u64;
typedef unsigned int uint; // for when you don't care about the size of your int.

typedef uintptr_t    usize;
typedef intptr_t     size;

typedef float        f32;
typedef double       f64;
typedef long double  f80; // architecture dependant i believe.

typedef char* str; // just more convenient to type

typedef u8 b8;
#ifndef bool
typedef uint bool;
#endif

#define true 1
#define false 0

#define fnptr(type, name, ...) type (*name)(__VA_ARGS__)
