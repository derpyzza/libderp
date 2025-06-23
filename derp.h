/*
 * This software is dual-licensed under the MIT License and The Unlicense.
 * You may choose whichever license you prefer.
 *
 * See LICENSE.MIT and LICENSE.UNLICENSE for details.
*/
#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef int8_t       i8;
typedef int16_t      i16;
typedef int32_t      i32;
typedef int64_t      i64;
typedef uint8_t      u8;
typedef uint16_t     u16;
typedef uint32_t     u32;
typedef uint64_t     u64;
typedef unsigned int uint; // for when you don't care about the size of your int.

typedef size_t       usize;
typedef intptr_t     isize;

typedef float        f32;
typedef double       f64;
typedef long double  f80; // architecture dependant i believe.


#define in_range_inc(v, min, max) ((min <= v) && (v <= max))
#define in_range_ex(v, min, max)  ((min < v) && (v < max))
#define fnptr(type, name, ...) type (*name)(__VA_ARGS__)
// convenience macro for checking null pointer dereferences
#define chkptr(ptr) if (ptr == NULL) { dlog_fatal("ptr is null"); exit(-1); }
#define fatal_panic(msg, err) { dlog_fatal(msg); exit(err); }

#define dres_decl(t, n) typedef struct dres_##n { t value; int ok; } dres_##n;
// inline
#define dres(t) struct { t value; int ok; }

#ifdef DEBUG
  #if _MSC_VER
    #include <intrin.h>
    #define dassert(c) if(c) {} else __debugbreak()
  #elif __GNUC__
    #define dassert(c) if(c) {} else { assert_fail(#c, __FILE__, __LINE__);  __builtin_trap(); }
  #else
    #define dassert(c) if ((c)) {} else *(volatile int *)0 = 0
  #endif
  void assert_fail (const char* expr, const char* file, int line);
#else 
  #define dassert(expr)
#endif

// memory management shenanigans
// 

typedef struct d_allocator {
  void *(*alloc)(isize, void*);
  void (*free)(void*, void*);
  void * ctx;
} d_allocator;

void * default_malloc(isize l, void*);
void   default_free(void*, void*);

extern d_allocator def_allocator;

static inline void * d_alloc(isize l) {
  return def_allocator.alloc(l, def_allocator.ctx);
}

static inline void d_free(void * ptr) {
  def_allocator.free(ptr, def_allocator.ctx);
}



// virtually expanding memory buffer
typedef struct d_vmem {
  u8* buf;  
  isize allocated,
        size;
#if PLATFORM_WINDOWS
  isize comitted,
#endif
} d_vmem;

// arena allocator
typedef struct d_arena {
  u8* buf;  
  isize allocated, // total allocated bytes
        size;      // buffer size
  int allocations; // total allocations done
} d_arena;

// type-unsafe dynamic array
typedef struct dbuf {
	void** data;     // the data array itself
	usize cap        // Max allocated size	
	   , current     // Current item || length of the array
	   , elem_size;  // size of one individual element, for alignment purposes
} dbuf;

// nicer string type
typedef struct dstr {
	char* cptr;
	isize len;
} dstr;

typedef struct dfilepath {
	dstr full_path; 	// the full path, including the extension
	dstr path; 			// the path, excluding the extension
	dstr name;			 	// the file name
	dstr ext; 				// the file extension
} dfilepath;

// rgb colours
typedef struct drgb  { u8  r, g, b; } drgb;   // 8 bits per colour
typedef struct drgbf { f32 r, g, b; } drgbf;  // normalized colour 0-1
// rgb + alpha colours
typedef struct drgba { u8  r, g, b, a; } drgba;   // 8 bits per colour
typedef struct drgbaf { f32 r, g, b, a; } drgbaf; // normalized colour 0-1

typedef enum {
	DLOG_FATAL = 0,
	DLOG_ERROR,
	DLOG_WARN,
	DLOG_INFO,
	DLOG_DEBUG,
	DLOG_TODO,
	DLOG_NUMBER
} DLogLevel;

// for now the log levels are just hardcoded into the code.
// later i plan on making a nicer logging system but this is good enough for now
//
// warning: "good enough for now" often translates to "this is a going to be a permanent feature"
#ifdef DEBUG
#define DLOG_LEVEL DLOG_DEBUG
#else
#define DLOG_LEVEL DLOG_ERROR
#endif

#define dlog_fatal(...) dlog_log(DLOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)
#define dlog_error(...) dlog_log(DLOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define dlog_warn(...)	dlog_log(DLOG_WARN , __FILE__, __LINE__, __VA_ARGS__)
#define dlog_info(...)	dlog_log(DLOG_INFO , __FILE__, __LINE__, __VA_ARGS__)
#define dlog_debug(...) dlog_log(DLOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define dlog_todo(...)  dlog_log(DLOG_TODO , __FILE__, __LINE__, __VA_ARGS__)

void dlog_log(DLogLevel log_level, const char* file, int line, const char* fmt, ...);
// void log_init(LogLevel level);

void dvmem_init(d_vmem *buf, isize mb_size);
void * dvmem_alloc(d_vmem *buf, isize len);
void dvmem_free(d_vmem* buf);

// initializes an arena object
void darena_init (d_arena *buf, isize size);
// initializes and allocates memory for an arena object
d_arena *darena_init_alloc (isize size);

// allocates memory given an arena
void * darena_alloc (d_arena *buf, isize len);
void darena_free (d_arena* buf);

// === DBUF ===
/**
	@param type — type of data
	@param size — initial array size 
*/
#define dbuf_new(type, size) dbuf_make_new(size, sizeof(type))
#define dbuf_init(type, size, data) _init_dbuf(data, size, sizeof(type))
dbuf dbuf_make_new(isize init, isize elem_size);
dbuf dbuf_init_new(void* data, isize len, isize elem_size);
// returns pointer to current item in list
void *dbuf_getc(dbuf* v);
int dbuf_grow(dbuf* buf, isize size);
int dbuf_push(dbuf *v, void* item);
void *dbuf_pop(dbuf *v);


// === DSTR ===

#define dstr(b) (dstr){ b, sizeof(b) }
// create new string, with an initial size of `init`
dstr dstr_new(isize init);
// copies string data from 'from' to 'to'
void dstr_copy(dstr* to, dstr* from);
// creates new string from a given c string
dstr dstr_from(char* from);
// duplicates a string s and returns a copy
dstr dstr_dup (dstr s);
// checks if the given strins are equal in length
bool dstr_eq(dstr a, dstr b);
// append d-string `src` to d-string `dest`
dstr dstr_concat(dstr dest, dstr* src);
// append c string ( char * ) `s` to `o`
int dstr_append_cstr(dstr *to, const char* from);

dbuf dstr_split_lines   (dstr src);
dbuf dstr_split_n_lines (dstr src, isize n); // TODO: unimplemented
dbuf dstr_split_tokens  (dstr src, const char* tkn);

int dstr_parse_int(dstr src);   // TODO: Unimplemented
int dstr_parse_float(dstr src); // TODO: Unimplemented

// === DFILE ===

u64 dfile_get_size(char* path);

dstr dfile_read           (char* file_path);
dstr dfile_read_bytes     (char* file_path, u64 bytes);
dbuf dfile_read_lines     (char* file_path);
dbuf dfile_read_n_lines   (char* file_path, u64 num_lines); // TODO: Unimplemented

int dfile_write           (char* file_path, dstr buf); // TODO: Unimplemented
int dfile_write_bytes     (char* file_path, dstr buf, isize bytes); // TODO: Unimplemented
int dfile_write_lines     (char* file_path, int num_lines, dstr* buf, u64* buf_len); // TODO: Unimplemented

dfilepath split_path(char* path);

// === DCOLOUR ===

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

// === MISC FUNCITONS ===

// ASCII only for now
bool char_is_digit    (char c);
bool char_is_alpha    (char c);
u8   char_to_digit    (char c);
bool char_is_alphanum (char c);


// T is the base type, I is the name for the outputted datatype ( eg for when you have a char* vec, but you want to call it a string vec )
#define dbuf_decl(T, N)                                                        \
  typedef struct dbuf_##N {                                                    \
    isize cap, current;                                                        \
    T *data;                                                                   \
  } dbuf_##N;                                                                  \
                                                                               \
  static inline dbuf_##N *dbuf_new_##N(isize init) {                           \
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
  static inline int dbuf_grow_##N(dbuf_##N *v, isize s) {                      \
    v->cap += s;                                                               \
    v->data = (T *)realloc(v->data, sizeof(T) * v->cap);                       \
    if (v->data)                                                               \
      return 0;                                                                \
    return -1;                                                                 \
  }                                                                            \
                                                                               \
  static inline T dbuf_getc_##N(dbuf_##N *v) { return v->data[v->current]; }   \
                                                                               \
  static inline void dbuf_push_##N(dbuf_##N *v, T i) {                         \
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
