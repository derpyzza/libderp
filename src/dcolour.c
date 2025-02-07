#include "../include/dcolour.h"

drgbf
drgb_to_drgbf (drgb c) {
  return (drgbf) {
  .r = (f32)((f32)c.r / 255.0f),
  .g = (f32)((f32)c.g / 255.0f),
  .b = (f32)((f32)c.b / 255.0f),
 };
}

drgbaf
drgba_to_drgbaf (drgba c) {
  return (drgbaf) {
  .r = (f32)((f32)c.r / 255.0f),
  .g = (f32)((f32)c.g / 255.0f),
  .b = (f32)((f32)c.b / 255.0f),
  .a = (f32)((f32)c.a / 255.0f),
 };
}

drgb 
drgbf_to_drgb (drgbf c) {
  return (drgb) {
  .r = (u8)(c.r * 255),
  .g = (u8)(c.g * 255),
  .b = (u8)(c.b * 255),
 };
}

drgba 
drgbaf_to_drgb (drgbaf c) {
  return (drgba) {
  .r = (u8)(c.r * 255),
  .g = (u8)(c.g * 255),
  .b = (u8)(c.b * 255),
 };
}

drgb 
drgb_from_hex(u32 c){
  drgb n = { 0 };
  n.r = (c >> 24) & 0xFF;
  n.g = (c >> 16) & 0xFF;
  n.b = (c >>  8) & 0xFF;
  return n;
}

drgba
drgba_from_hex(u32 c){
  drgba n = { 0 };
  n.r = (c >> 24) & 0xFF;
  n.g = (c >> 16) & 0xFF;
  n.b = (c >>  8) & 0xFF;
  n.b = c & 0xFF;
  return n;
}

drgbf
drgbf_from_hex(u32 c) {
  drgbf n = { 0 };
  n.r = (float)(((c >> 24) & 0xFF) / 255.0f);
  n.g = (float)(((c >> 16) & 0xFF) / 255.0f);
  n.b = (float)(((c >>  8) & 0xFF) / 255.0f);
  n.a = (float)((c & 0xFF) / 255.0f);
  return n;
}

drgbaf
drgbaf_from_hex(u32 c) {  
  drgbaf n = { 0 };
  n.r = (float)(((c >> 24) & 0xFF) / 255.0f);
  n.g = (float)(((c >> 16) & 0xFF) / 255.0f);
  n.b = (float)(((c >>  8) & 0xFF) / 255.0f);
  n.a = (float)((c & 0xFF) / 255.0f);
  return n;
}

u32 
drgb_to_hex (drgb c) {  
  u32 col = 0;
  col |= ( c.r << 24 );
  col |= ( c.g << 16 );
  col |= ( c.b <<  8 );
  return col;
}

u32
drgba_to_hex (drgba c) {
  u32 col = 0;
  col |= ( c.r << 24 );
  col |= ( c.g << 16 );
  col |= ( c.b <<  8 );
  col |= ( c.a );
  return col;
}

u32 
drgbf_to_hex (drgbf c) {
  u32 col = 0;
  col |= ((u8) ( c.r * 255 ) << 24 );
  col |= ((u8) ( c.g * 255 ) << 16 );
  col |= ((u8) ( c.b * 255 ) <<  8 );
  return col;
}

u32 
drgbaf_to_hex (drgbaf c) {
  u32 col = 0;
  col |= ((u8) ( c.r * 255 ) << 24 );
  col |= ((u8) ( c.g * 255 ) << 16 );
  col |= ((u8) ( c.b * 255 ) <<  8 );
  col |= ((u8) ( c.a * 255 ));
  return col;
}
