/*
This software is dual-licensed under the MIT License and The Unlicense.
You may choose whichever license you prefer.

See LICENSE.MIT and LICENSE.UNLICENSE for details.
*/
#pragma once

#include "derp.h"

// rgb colours
typedef struct drgb     drgb;   // 8 bits per colour
typedef struct drgbf    drgbf;  // normalized colour 0-1
// rgb + alpha colours
typedef struct drgba    drgba;   // 8 bits per colour
typedef struct drgbaf   drgbaf;  // normalized colour 0-1

struct drgba  { u8  r, g, b, a; };
struct drgbaf { f32 r, g, b, a; };
struct drgb   { u8  r, g, b, a; };
struct drgbf  { f32 r, g, b, a; };

drgb  drgbf_to_drgb  (drgbf);
drgbf drgb_to_drgbf  (drgb);
drgba drgbf_to_drgba (drgbf);
drgbf drgba_to_drgbf (drgba);

drgb   drgb_from_hex   (u32 hex);
drgbf  drgbf_from_hex  (u32 hex);
drgba  drgba_from_hex  (u32 hex);
drgbaf drgbaf_from_hex (u32 hex);

u32 drgb_to_hex (drgb);
u32 drgbf_to_hex (drgbf);
u32 drgba_to_hex (drgba);
u32 drgbaf_to_hex (drgbaf);
