/*
This software is dual-licensed under the MIT License and The Unlicense.
You may choose whichever license you prefer.

See LICENSE.MIT and LICENSE.UNLICENSE for details.
*/

// General string and buffer management functions
// includes functionality for strings, dynamic arrays, and file I/O
#pragma once
#include <stdlib.h>

#include "derp.h"

// type-unsafe dynamic array
typedef struct dbuf {
	usize cap        // Max allocated size	
	   , current     // Current item || length of the array
	   , elem_size;  // size of one individual element, for alignment purposes
	void** data;     // the data array itself
} dbuf;

// Dynamically sized string
typedef struct dstr {
	usize len;
	usize cap;
	char* cptr;
} dstr;

typedef struct dfilepath {
	char* full_path; 	// the full path, including the extension
	char* path; 			// the path, excluding the extension
	char* name;			 	// the file name
	char* ext; 				// the file extension
} dfilepath;

// === DBUF ===
/**
	@param type — type of data
	@param size — initial array size 
*/
#define dbuf_new(type, size) dbuf_make_new(size, sizeof(type))
#define dbuf_init(type, size, data) _init_dbuf(data, size, sizeof(type))

// T is the base type, I is the name for the outputted datatype ( eg for when you have a char* vec, but you want to call it a string vec )
#define dbuf_decl(T, N)                                                        \
  typedef struct dbuf_##N {                                                    \
    size cap, current;                                                         \
    T *data;                                                                   \
  } dbuf_##N;                                                                  \
                                                                               \
  static inline dbuf_##N *dbuf_##N_new(size init) {                            \
    dbuf_##N *v = (dbuf_##N *)malloc(sizeof(dbuf_##N));                        \
    if (v) {                                                                   \
      v->cap = init;                                                           \
      v->current = 0;                                                          \
      v->data = (T *)calloc(v->cap, sizeof(T));                                \
      return v;                                                                \
    }                                                                          \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  static inline int dbuf_##N_grow(dbuf_##N *v, size s) {                       \
    v->cap += s;                                                               \
    v->data = (T *)realloc(v->data, sizeof(T) * v->cap);                       \
    if (v->data)                                                               \
      return 0;                                                                \
    return -1;                                                                 \
  }                                                                            \
                                                                               \
  static inline void dbuf_##N_push(dbuf_##N *v, T i) {                         \
    if (v->current + 1 > v->cap) {                                             \
      v->cap *= 2;                                                             \
      v->data = (T *)realloc(v->data, sizeof(T) * v->cap);                     \
    }                                                                          \
    v->data[v->current] = i;                                                   \
    v->current++;                                                              \
  }                                                                            \
                                                                               \
  static inline T *vec_pop_##N(dbuf_##N *v) {                                  \
    return (T *)&v->data[(--v->current)];                                      \
  }

dbuf *dbuf_make_new(size init, size elem_size);
dbuf *dbuf_init_new(void* data, size len, size elem_size);
int dbuf_grow(dbuf* buf, size size);
int dbuf_push(dbuf *v, void* item);
void *dbuf_pop(dbuf *v);


// === DSTR ===

// create new dynamic string, with an initial size of `init`
dstr *dstr_new(size init);
// create new dynamic string, using the string `s` as the base string
dstr *dstr_from(const char* s);
// append d-string `s` to d-string `o`
void dstr_append_dstr(dstr* o, const dstr* s);
// append c string ( char * ) `s` to `o`
void dstr_append_cstr(dstr* o, const char* s);
dstr* dstr_dup (const dstr* s);

// ASCII only for now
bool char_is_digit(char c);
bool char_is_alpha(char c);    // TODO: Unimplemented
bool char_is_alphanum(char c); // TODO: Unimplemented
u8 char_to_digit(char c);

int dstr_parse_int(dstr src);   // TODO: Unimplemented
int dstr_parse_float(dstr src); // TODO: Unimplemented

dbuf* dstr_split_lines  (dstr *src);
dbuf* dstr_split_tokens (dstr *src, const char* tkn);

// === DFILE ===

u64 dfile_get_size(char* path);

dstr *dfile_read           (char* file_path);
dstr *dfile_read_bytes     (char* file_path, u64 bytes);
dbuf *dfile_read_lines     (char* file_path);
dbuf *dfile_read_n_lines   (char* file_path, u64 num_lines);                           // TODO: Unimplemented
int dfile_write            (char* file_path, dstr* buf);                               // TODO: Unimplemented
int dfile_write_bytes      (char* file_path, dstr* buf, size bytes);                   // TODO: Unimplemented
int dfile_write_lines      (char* file_path, int num_lines, dstr** buf, u64* buf_len); // TODO: Unimplemented

dfilepath split_path(char* path);
