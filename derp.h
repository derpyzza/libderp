/*
This software is dual-licensed under the MIT License and The Unlicense.
You may choose whichever license you prefer.

See LICENSE.MIT and LICENSE.UNLICENSE for details.
*/
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define IN_RANGE_INC(v, min, max) ((min <= v) && (v <= max))
#define IN_RANGE_EX(v, min, max)  ((min < v) && (v < max))
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

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

typedef char* str; // just more convenient to type in some cases, but char * is clearer

#define fnptr(type, name, ...) type (*name)(__VA_ARGS__)
