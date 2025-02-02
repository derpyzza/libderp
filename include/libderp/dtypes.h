#pragma once

#include <stdint.h>
#include <stdbool.h>

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
// size types are signed by default, as that is handy for error detection in the case of
// overflow.
// the code doesn't know if you accidentally used a large size or if you *really* wanted
// a large size, but it certainly does know that you probably don't want
// a negatively sized size.
typedef intptr_t     size;

typedef float        f32;
typedef double       f64;
typedef long double  f80; // architecture dependant i believe.

typedef char* str; // just more convenient to type in some cases, but char * is clearer

typedef u8 b8;

#define true 1
#define false 0

#define fnptr(type, name, ...) type (*name)(__VA_ARGS__)

typedef struct result {
	int err;
	void *ok;
} result;
