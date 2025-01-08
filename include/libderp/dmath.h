#pragma once

#include "dtypes.h"

#define EPSILON 0.001f
#ifndef PI
#define PI 3.141592653589f
#endif
#define PHI 1.61803399f
#define HALF_PI 1.570796326794f
#define DEG_TO_RAD 0.0174532925f
#define RAD_TO_DEG 57.2957795131

// handy mathy macro functions
#define IN_RANGE_INC(v, min, max) ((min <= v) && (v <= max))
#define IN_RANGE_EX(v, min, max)  ((min < v) && (v < max))
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

typedef struct { f32 x, y;       } Vec2;
typedef struct { f32 x, y, z;    } Vec3;
typedef struct { f32 x, y, z, w; } Vec4;
typedef struct { int n; f32 *d;  } VecN; // N-Dimentional Vector

typedef struct { f32 x, y, w, h; } Rect;
typedef struct { f32 x, y, r;    } Circ;
