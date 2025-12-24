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

typedef struct d_alloc_data {
  const char* file;
  int line_num;
  void* ctx;
} d_alloc_data;

typedef struct d_allocator {
  void *(*alloc)(isize len, bool clear, d_alloc_data data);
  void (*free)(void* ptr, isize len, d_alloc_data data);
  void * ctx;
} d_allocator;

extern d_allocator def_allocator;

// set default allocator for this program
void dalloc_set_default(d_allocator alloc);

#define d_alloc(len) def_allocator.alloc(len, false, (d_alloc_data){__FILE__, __LINE__, def_allocator.ctx})
#define d_calloc(len, elem) def_allocator.alloc(len * elem, true, (d_alloc_data){__FILE__, __LINE__, def_allocator.ctx})
#define d_free(ptr, len) def_allocator.free(ptr, len, (d_alloc_data){__FILE__, __LINE__, def_allocator.ctx})

// static inline void * d_alloc(isize l) {
//   return def_allocator.alloc(l, false, def_allocator.ctx);
// }

// static inline void * d_calloc(isize num, isize elem_size) {
//   return def_allocator.alloc(num * elem_size, true, def_allocator.ctx);
// }

// static inline void d_free(void * ptr, isize len) {
//   def_allocator.free(ptr, len, def_allocator.ctx);
// }

void * d_tracking_alloc(isize l, bool clear, d_alloc_data data);
void d_tracking_free(void* ptr, isize len, d_alloc_data data);

typedef struct dtrack_allocator_data {
  int num_allocations;
  isize memory_allocated;
  isize memory_freed;
} dtrack_alloc_data;

static d_allocator dtracking_allocator = {
  .alloc = d_tracking_alloc,
  .free = d_tracking_free,
};

static inline void dtracking_alloc_init(dtrack_alloc_data * data) {
  dtracking_allocator.ctx = data;
}

// arena allocator
typedef struct d_arena {
  u8* buf;  
  isize allocated, // total allocated bytes
        size;      // buffer size
  int allocations; // total allocations done
} d_arena;

// type-unsafe dynamic array
// NOTE: it's recommended to use DBUF_DECL to create a new dbuf type instead of using this
typedef struct dbuf {
	void** data;     // the data array itself
	usize cap        // Max allocated size	
	    , len        // Current item || length of the array
	    , elem_size; // size of one individual element, for alignment purposes
} dbuf;
// just to make it nice and apparant what the underlying element type is
#define dbuf(item) dbuf

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

#define DLOG_LEVEL_DATA \
  DLOG(FATAL, "FTAL", "\x1b[35m")\
  DLOG(ERROR, "ERRO", "\x1b[31m")\
  DLOG(WARN,  "WARN", "\x1b[32m")\
  DLOG(INFO,  "INFO", "\x1b[36m")\
  DLOG(DEBUG, "DEBG", "\x1b[33m")\
  DLOG(TRACE, "TRAC", "\x1b[34m")

typedef enum {
	#define DLOG(e, s, c) DLOG_##e,
	DLOG_LEVEL_DATA
	#undef DLOG
	DLOG_NUMBER
} DLogLevel;

static struct dlogger {
  DLogLevel level;
  FILE * f_out;
} def_logger;

typedef void (*logFunc)(DLogLevel, const char* file, int line, const char* fmt, ...);

#define dlog_fatal(...) dlog_func(DLOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)
#define dlog_error(...) dlog_func(DLOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define dlog_warn(...)	dlog_func(DLOG_WARN , __FILE__, __LINE__, __VA_ARGS__)
#define dlog_info(...)	dlog_func(DLOG_INFO , __FILE__, __LINE__, __VA_ARGS__)
#define dlog_debug(...) dlog_func(DLOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define dlog_trace(...) dlog_func(DLOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)

// initialize logger
void dlog_init(DLogLevel level);
// set output stream for the default logger
void dlog_set_output(FILE * f);
// logging function
void dlog_func(DLogLevel log_level, const char* file, int line, const char* fmt, ...);

// initializes an arena object
void darena_init (d_arena *buf, isize size);
// initializes and allocates memory for an arena object
d_arena darena_init_alloc (isize size);

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

#define dbuf_get(v, i) v.data[i]

dbuf dbuf_make_new(isize init, isize elem_size);
dbuf dbuf_init_new(void* data, isize len, isize elem_size);
// returns pointer to current item in list
void *dbuf_getc(dbuf v);
int dbuf_grow(dbuf buf, isize size);
int dbuf_push(dbuf v, void* item);
void *dbuf_pop(dbuf v);
// return a pointer to the element on the given index in the dbuf
// void *dbuf_get(isize index);


// === DSTR ===

#define dstr(b) (dstr){ .cptr = (b), .len = sizeof((b)) - 1 }
#define dstr_fmt(b) (int)(b).len, (b).cptr
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

// === MISC FUNCITONS ===

// ASCII only for now
bool d_char_is_digit    (char c);
bool d_char_is_alpha    (char c);
u8   d_char_to_digit    (char c);
bool d_char_is_alphanum (char c);


// T is the base type, N is the name for the outputted datatype ( eg for when you have a char* vec, but you want to call it a string vec )
#define dbuf_decl(T, N)                                                        \
  typedef struct dbuf_##N {                                                    \
    isize cap, len;                                                            \
    T *data;                                                                   \
  } dbuf_##N;                                                                  \
                                                                               \
  static inline dbuf_##N dbuf_new_##N(isize init) {                            \
    dbuf_##N v = {0};                                                          \
                                                                               \
    v.cap = init;                                                              \
    v.len = 0;                                                                 \
    v.data = (T *)calloc(v.cap, sizeof(T));                                    \
    return v;                                                                  \
  }                                                                            \
                                                                               \
  static inline dbuf_##N dbuf_new_from_##N(T *data, isize len) {               \
    dbuf_##N v = dbuf_new_##N(len);                                            \
                                                                               \
    if (!data) {                                                               \
      dlog_error("Inputted data is NULL, aborting");                           \
      return v;                                                                \
    }                                                                          \
                                                                               \
    memcpy(data, v.data, sizeof(T) * len);                                     \
                                                                               \
    if (!v.data) {                                                             \
      dlog_error("Couldn't create new vec, copy fail");                        \
    }                                                                          \
    return v;                                                                  \
  }                                                                            \
                                                                               \
  static inline int dbuf_grow_##N(dbuf_##N v, isize s) {                       \
    v.cap += s;                                                                \
    v.data = (T *)realloc(v.data, sizeof(T) * v.cap);                          \
    if (v.data) {                                                              \
      return 0;                                                                \
    }                                                                          \
    return -1;                                                                 \
  }                                                                            \
                                                                               \
  static inline T dbuf_getc_##N(dbuf_##N v) { return v.data[v.len]; }          \
                                                                               \
  static inline void dbuf_push_##N(dbuf_##N v, T i) {                          \
    if (v.len + 1 > v.cap) {                                                   \
      v.cap *= 2;                                                              \
      v.data = (T *)realloc(v.data, sizeof(T) * v.cap);                        \
    }                                                                          \
    v.data[v.len] = i;                                                         \
    v.len++;                                                                   \
  }                                                                            \
                                                                               \
  static inline T *vec_pop_##N(dbuf_##N v) {                                   \
    if(v.len == 0) { return NULL; }                                            \
    return (T *)&v.data[(--v.len)];                                            \
  }
