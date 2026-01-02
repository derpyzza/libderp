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

#define dassert(c, ...) if((c)) {} else { assert_fail( #c, __FILE__, __LINE__, __VA_ARGS__); }
void assert_fail (const char* expr, const char* file, int line, const char* msg, ...) __attribute__((format(printf, 4, 5)));


#define in_range_inc(v, min, max) ((min <= v) && (v <= max))
#define in_range_ex(v, min, max)  ((min < v) && (v < max))
#define fnptr(type, name, ...) type (*name)(__VA_ARGS__)

// array element iterator
// type: type of element
// item: item name ( pointer )
// array: array
// len: length of array
//
// example use:
// int count = 10
// int * values = get_values(count);
// dforeach(int, value, values, count) {
//   printf("value is: %i", *value);
// }
#define dforeach(type, item, array, len) \
  for ( type * (item) = (array); (item) < (array) + (len); (item)++ )

// indexed array element iterator
// type: type of element
// item: item name ( pointer )
// index: current index
// array: array
// len: length of array
//
// example use:
// int count = 10
// int * values = get_values(count);
// dforeach_id(int, value, id, values, count) {
//   printf("value at %i is: %i", id, *value);
// }
#define dforeach_id(type, item, index, array, len) \
  int (index) = 0; \
  for ( type * (item) = (array); (item) < (array) + (len); (item)++, (index)++ )

// memory management shenanigans
// 

typedef struct d_alloc_data {
  const char* file;
  int line_num;
  void* ctx;
} d_alloc_data;

typedef struct d_allocator {
  fnptr(void*, alloc, isize len, bool clean, d_alloc_data data);
  fnptr(void*, realloc, void * block, isize len, d_alloc_data data);
  fnptr(void, free, void* ptr, d_alloc_data data);
  void * ctx;
} d_allocator;

// default global program allocator
extern d_allocator def_allocator;

// set default / global allocator for this program
void dalloc_set_default(d_allocator alloc);

// === General Allocator Macros ===
// allocate using whatever the global allocator is set to.
#define d_alloc(len) def_allocator.alloc(len, false, (d_alloc_data){__FILE__, __LINE__, def_allocator.ctx})
#define d_calloc(len, elem) def_allocator.alloc(len * elem, true, (d_alloc_data){__FILE__, __LINE__, def_allocator.ctx})
#define d_realloc(data, len) def_allocator.realloc(data, len, (d_alloc_data){__FILE__, __LINE__, def_allocator.ctx})
#define d_free(ptr) def_allocator.free(ptr, (d_alloc_data){__FILE__, __LINE__, def_allocator.ctx})
// === General Allocator Macros ===

// === Allocator Macros ===
// allocate using the given allocator
#define dalloc_alloc(alloca, size) alloca.alloc(size, false, (d_alloc_data){__FILE__, __LINE__, alloca.ctx })
#define dalloc_calloc(alloca, len, elem) alloca.alloc( (len * elem), true, (d_alloc_data){__FILE__, __LINE__, alloca.ctx })
#define dalloc_realloc(alloca, data, size) alloca.realloc(data, size, (d_alloc_data){__FILE__, __LINE__, alloca.ctx })
#define dalloc_free(alloca, ptr) alloca.free(ptr, (d_alloc_data){__FILE__, __LINE__, alloca.ctx })
// === Allocator Macros ===

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
	void* data;      // the data array itself
	isize len        // Max allocated size	
	    , cur        // Current item || length of the array
	    , elem_size; // size of one individual element, for alignment purposes
	d_allocator * alloc;
} dbuf;
// just to make it nice and apparant what the underlying element type is
#define dbuf(item) dbuf_##item

// dbuf element iterator
// type: type of element
// item: item name ( pointer )
// dbuf: dbuf
//
// example use:
// dbuf_ints nums = get_nums();
// dforeach(int, num, nums) {
//   printf("num is: %i", *num);
// }
#define dbuf_foreach(type, item, dbuf) \
  for ( type * (item) = (dbuf).data; (item) < (dbuf).data + (dbuf).cur; (item)++ )

// indexed dbuf element iterator
// type: type of element
// item: item name ( pointer )
// index: current index
// dbuf: dbuf
//
// example use:
// dbuf_ints nums = get_nums();
// dforeach_id(int, num, id, nums) {
//   printf("num at %i is: %i", id, *num);
// }
#define dbuf_foreach_id(type, item, index, dbuf) \
  int (index) = 0; \
  for ( type * (item) = (dbuf).data; (item) < (dbuf).data + (dbuf).cur; (item)++, (index)++ )

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
  DLOG(INVALID, "NULL", "\x1b[35m")\
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
void dlog_func(DLogLevel log_level, const char* file, int line, const char* fmt, ...) __attribute__((format(printf, 4, 5)));

// initializes an arena object
void darena_init (d_arena *buf, isize size);
// initializes and allocates memory for an arena object
d_arena darena_init_alloc (isize size);

// allocates memory given an arena
void * darena_alloc (d_arena *buf, isize len);
void darena_free (d_arena* buf);


typedef struct dbuf_data {
  void ** data; // pointer to array of void*
  isize *len, *cur;
  isize elem_size;
  d_allocator * alloc;
} dbuf_data;

// === DBUF ===
#define dbuf_make(vecptr, size)                                                \
  _dbuf_make(size, (dbuf_data){.data = (void **)&(vecptr)->data,               \
                               .len = &(vecptr)->len,                          \
                               .cur = &(vecptr)->cur,                          \
                               .elem_size = sizeof((*vecptr).data),            \
                               .alloc = &(vecptr)->alloc})
#define dbuf_init(vecptr, from, size)                                          \
  _dbuf_init((void **)from, size,                                              \
             (dbuf_data){.data = (void **)&(vecptr)->data,                     \
                         .len = &(vecptr)->len,                                \
                         .cur = &(vecptr)->cur,                                \
                         .elem_size = sizeof((*vecptr).data),                  \
                         .alloc = &(vecptr)->alloc})

#define dbuf_make_alloc(vecptr, _size, _alloc)                                 \
  _dbuf_make(_size, (dbuf_data){.data = (void **)&(vecptr)->data,              \
                                .len = &(vecptr)->len,                         \
                                .cur = &(vecptr)->cur,                         \
                                .elem_size = sizeof((*vecptr).data),           \
                                .alloc = &_alloc})
#define dbuf_init_alloc(vecptr, _from, _size, _alloc)                          \
  _dbuf_init((void **)_from, _size,                                            \
             (dbuf_data){.data = (void **)&(vecptr)->data,                     \
                         .len = &(vecptr)->len,                                \
                         .cur = &(vecptr)->cur,                                \
                         .elem_size = sizeof((*vecptr).data),                  \
                         .alloc = &_alloc})
#define dbuf_push(vecptr, item)                                                \
  _dbuf_push(&item, (dbuf_data){.data = (void **)&(vecptr)->data,              \
                                .len = &(vecptr)->len,                         \
                                .cur = &(vecptr)->cur,                         \
                                .elem_size = sizeof((*vecptr).data),           \
                                .alloc = &(vecptr)->alloc})
#define dbuf_pop(vecptr)                                                       \
  _dbuf_pop((dbuf_data){.data = (void **)&(vecptr)->data,                      \
                        .len = &(vecptr)->len,                                 \
                        .cur = &(vecptr)->cur,                                 \
                        .elem_size = sizeof((*vecptr).data),                   \
                        .alloc = &(vecptr)->alloc})
#define dbuf_get(vecptr, id)                                                   \
  _dbuf_get(id, (dbuf_data){.data = (void **)&(vecptr)->data,                  \
                            .len = &(vecptr)->len,                             \
                            .cur = &(vecptr)->cur,                             \
                            .elem_size = sizeof((*vecptr).data),               \
                            .alloc = &(vecptr)->alloc})
#define dbuf_set(vecptr, id, item)                                             \
  _dbuf_set((void *)item, id,                                                  \
            (dbuf_data){.data = (void **)&(vecptr)->data,                      \
                        .len = &(vecptr)->len,                                 \
                        .cur = &(vecptr)->cur,                                 \
                        .elem_size = sizeof((*vecptr).data),                   \
                        .alloc = &(vecptr)->alloc})
#define dbuf_insert(vecptr, id, item)                                          \
  _dbuf_insert((void *)item, id,                                               \
               (dbuf_data){.data = (void **)&(vecptr)->data,                   \
                           .len = &(vecptr)->len,                              \
                           .cur = &(vecptr)->cur,                              \
                           .elem_size = sizeof((*vecptr).data),                \
                           .alloc = &(vecptr)->alloc})
#define dbuf_free(vecptr) \
  _dbuf_free((dbuf_data){.data = (void**)&(vecptr)->data, .alloc = &(vecptr)->alloc})

// make new dbuf of size `size`
void _dbuf_make ( isize size, dbuf_data data );
// initialize new dbuf from array `from`, of size `size`
void _dbuf_init ( void **from, isize size, dbuf_data data );

// make new dbuf of size `size` with allocator `alloc`
void _dbuf_make_alloc ( isize size, d_allocator alloc, dbuf_data data );
// initialize new dbuf from array `from`, of size `size`, with allocator `alloc`
void _dbuf_init_alloc ( void **from, isize size, d_allocator alloc, dbuf_data data );

// push `item` into dbuf
void _dbuf_push ( void *item, dbuf_data data );
// insert `item` at `index`
void _dbuf_insert ( void *item, isize index, dbuf_data data );
// pop item from dbuf
void* _dbuf_pop ( dbuf_data data );

// get item at `index`
void* _dbuf_get ( isize index, dbuf_data data );
// set item at `index` to `item`
void  _dbuf_set ( isize index, void *item, dbuf_data data );

// free dbuf
void _dbuf_free ( dbuf_data data );


// === DSTR ===

#define dstr(b) (dstr){ .cptr = (b), .len = sizeof((b)) - 1 }
#define dstr_fmt(b) (int)(b).len, (b).cptr
// create new string, with an initial size of `init`
dstr dstr_new(isize init);
// copies string data from 'from' to 'to'
void dstr_copy(dstr *to, dstr *from);
// creates new string from a given c string
dstr dstr_from(char *from);
// duplicates a string s and returns a copy
dstr dstr_dup (dstr s);
// checks if the given strins are equal in length
bool dstr_eq(dstr a, dstr b);
// append d-string `src` to d-string `dest`
dstr dstr_concat(dstr dest, dstr* src);
// append c string ( char * ) `s` to `o`
int dstr_append_cstr(dstr *to, const char* from);
// checks if a given dstr `str` starts with a string `st`
bool dstr_starts_with(dstr str, const char* st);

dbuf dstr_split_lines   (dstr src);
dbuf dstr_split_n_lines (dstr src, isize n); // TODO: unimplemented
dbuf dstr_split_tokens  (dstr src, const char* tkn);

int dstr_parse_int(dstr src);   // TODO: Unimplemented
float dstr_parse_float(dstr src); // TODO: Unimplemented

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
    isize cur, len;                                                            \
    T *data;                                                                   \
    d_allocator alloc;                                                         \
  } dbuf_##N;                                                                  \
                                                                               \
  static __attribute__((unused)) inline dbuf_##N dbuf_new_##N(isize init) {    \
    dbuf_##N v = {0};                                                          \
                                                                               \
    v.len = init;                                                              \
    v.cur = 0;                                                                 \
    v.data = (T *)calloc(v.len, sizeof(T));                                    \
    return v;                                                                  \
  }                                                                            \
                                                                               \
  static __attribute__((unused)) inline dbuf_##N dbuf_new_from_##N(            \
      T *data, isize len) {                                                    \
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
  static __attribute__((unused)) inline int dbuf_grow_##N(dbuf_##N v,          \
                                                          isize s) {           \
    v.len += s;                                                                \
    v.data = (T *)realloc(v.data, sizeof(T) * v.len);                          \
    if (v.data) {                                                              \
      return 0;                                                                \
    }                                                                          \
    return -1;                                                                 \
  }                                                                            \
                                                                               \
  static __attribute__((unused)) inline T dbuf_getc_##N(dbuf_##N v) {          \
    return v.data[v.cur];                                                      \
  }                                                                            \
                                                                               \
  static __attribute__((unused)) inline void dbuf_push_##N(dbuf_##N v, T i) {  \
    if (v.cur + 1 > v.len) {                                                   \
      v.len *= 2;                                                              \
      v.data = (T *)realloc(v.data, sizeof(T) * v.len);                        \
    }                                                                          \
    v.data[v.cur] = i;                                                         \
    v.cur++;                                                                   \
  }                                                                            \
                                                                               \
  static __attribute__((unused)) inline T *vec_pop_##N(dbuf_##N v) {           \
    if (v.cur == 0) {                                                          \
      return NULL;                                                             \
    }                                                                          \
    return (T *)&v.data[(--v.cur)];                                            \
  }
