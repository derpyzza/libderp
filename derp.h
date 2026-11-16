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

#ifdef __cplusplus
extern "C" {
#endif

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

#if defined(__GNUC__) || defined(__clang__)
#  define DATTR_UNUSED __attribute__((unused))
#  define DATTR_FORMAT(F, I) __attribute__((format(printf, F, I)))
#else
#  define DATTR_UNUSED
#  define DATTR_FORMAT(F, I) 
#endif


#define dassert(c, ...) if((c)) {} else { assert_fail( #c, __FILE__, __LINE__, __VA_ARGS__); }
#define todo() assert_fail( __func__, __FILE__, __LINE__, "UNIMPLEMENTED" )
void assert_fail (const char* expr, const char* file, int line, const char* msg, ...) DATTR_FORMAT(4, 5);


#define in_range_inc(v, min, max) ((min <= v) && (v <= max))
#define in_range_ex(v, min, max)  ((min < v) && (v < max))
#define fnptr(type, name, ...) type (*name)(__VA_ARGS__)
// power of two align up macro
#define d_align(x, a) ( (x) + ( (a) - 1 ) & ~( (a) - 1 ))

#ifndef MAX
  #define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
  #define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

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

#include "d_buf_macro.h"

// 
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
#define d_alloc(LEN)         def_allocator.alloc((LEN), false, (d_alloc_data){__FILE__, __LINE__, def_allocator.ctx})
#define d_calloc(LEN, ELEM)  def_allocator.alloc((LEN) * (ELEM), true, (d_alloc_data){__FILE__, __LINE__, def_allocator.ctx})
#define d_realloc(DATA, LEN) def_allocator.realloc((DATA), (LEN), (d_alloc_data){__FILE__, __LINE__, def_allocator.ctx})
#define d_free(PTR)          def_allocator.free((PTR), (d_alloc_data){__FILE__, __LINE__, def_allocator.ctx})
// === General Allocator Macros ===

// === Allocator Macros ===
// allocate using the given allocator
#define dalloc_alloc(ALLOCA, SIZE)         (ALLOCA).alloc(SIZE, false, (d_alloc_data){__FILE__, __LINE__, (ALLOCA).ctx })
#define dalloc_calloc(ALLOCA, LEN, ELEM)   (ALLOCA).alloc(((LEN) * (ELEM)), true, (d_alloc_data){__FILE__, __LINE__, (ALLOCA).ctx })
#define dalloc_realloc(ALLOCA, DATA, SIZE) (ALLOCA).realloc((DATA), (SIZE), (d_alloc_data){__FILE__, __LINE__, (ALLOCA).ctx })
#define dalloc_free(ALLOCA, PTR)           (ALLOCA).free((PTR), (d_alloc_data){__FILE__, __LINE__, (ALLOCA).ctx })
// === Allocator Macros ===

// arena allocator
// a simple arena allocator that uses a linked list of buckets for
// growing allocations

typedef struct d_bucket {
  struct d_bucket * next;
  isize index, size;

  isize data[];
} d_bucket;

typedef struct d_arena {
  int allocations; // total allocations done
  d_bucket * head;
  d_bucket * tail;

  isize bucket_size;

  d_allocator alloc;
} d_arena;

// type-unsafe dynamic array
// NOTE: it's recommended to use DBUF_DECL to create a new dbuf type instead of using this
typedef struct dbuf {
	void * data;      // the data array itself
	isize len        // Max allocated size	
	    , cur        // Current item || length of the array
	    , elem_size; // size of one individual element, for alignment purposes

	d_allocator alloc;
} dbuf;

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
#define dbuf_foreach(TYPE, ITEM, DBUF) \
  for ( TYPE * (ITEM) = (DBUF).data; (ITEM) < (DBUF).data + (DBUF).cur; (ITEM)++ )

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

// nicer string type, acts as a non-owning view into another buffer
typedef struct dstr {
	char* cptr;
	isize len;
} dstr;


// dynamic, growable, owning string
typedef struct dstring {
  char * cptr;
  isize cur, len;
  d_allocator alloc;
} dstring;

typedef struct dfilepath {
	dstr full_path; 	// the full path, including the extension
	dstr path; 			  // the path, excluding the extension
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
void dlog_func(DLogLevel log_level, const char* file, int line, const char* fmt, ...)DATTR_FORMAT(4, 5);

// initializes an arena object
d_arena darena_init (isize size, d_allocator alloc);

// allocates memory given an arena
void * darena_alloc (d_arena *buf, isize len);
// clears an arena for the next use
void darena_clear ( d_arena* buf );
// frees all allocated memory
void darena_free ( d_arena* buf );

d_allocator darena_make_allocator( d_arena * arena );

// === DBUF ===

// convenience wrapper struct over common dbuf data
// both the generic dbuf macros and the generated type-safe
// dbuf wrapper functions generated by dbuf_decl wrap over this
// struct
// contains pointers to the various fields of dbufs
typedef struct dbuf_data {
  void ** data_ptr; // pointer to the owner struct's void* array
  isize *len, *cur;
  isize elem_size;
  d_allocator alloc;
} dbuf_data;

// internal helper function used by dbuf_decl
void __populate_dbuf_data(dbuf_data * data, dbuf * n, isize elem_size);

// make new dbuf of size `size`
void _dbuf_make ( isize size, dbuf_data data );
// initialize new dbuf from array `from`, of size `size`
void _dbuf_init ( const void **from, isize size, dbuf_data data );

// make new dbuf of size `size` with allocator `alloc`
void _dbuf_make_alloc ( isize size, d_allocator alloc, dbuf_data data );
// initialize new dbuf from array `from`, of size `size`, with allocator `alloc`
void _dbuf_init_alloc ( const void **from, isize size, d_allocator alloc, dbuf_data data );

// push `item` into dbuf
void _dbuf_push ( void * item, dbuf_data data );
// pop item from dbuf
void* _dbuf_pop ( dbuf_data data );

// insert `item` at `index`
void _dbuf_insert ( void * item, isize index, dbuf_data data );
// remove `item` at `index`
void _dbuf_remove ( isize index, dbuf_data data );

// grow dbuf
// basically internal, not much practical use for it
// TODO: Maybe just make it internal?
void _dbuf_grow ( dbuf_data data );
// reserve space of length `len`
void _dbuf_reserve ( isize len, dbuf_data data );

// append array to end of dbuf
void _dbuf_append ( const void** data, isize len, dbuf_data dbuf );
// prepend array to start of dbuf
void _dbuf_prepend ( const void** data, isize len, dbuf_data dbuf );

// get item at `index`
void* _dbuf_get ( isize index, dbuf_data data );
// set item at `index` to `item`
void  _dbuf_set ( isize index, void *item, dbuf_data data );

// create a new cloned-copy of a dbuf
void _dbuf_clone ( dbuf_data dest, dbuf src );

// free dbuf
void _dbuf_free ( dbuf_data data );

// dbuf_print
// dbuf_sort
// dbuf_insert_array := insert N items into middle of array
// dbuf_remove_n_items := remove N items from middle of array

#define dbuf_make(vecptr, type, size)                                          \
  _dbuf_make(size, (dbuf_data){.data_ptr = (void **)&(vecptr)->data,           \
                               .len = &(vecptr)->len,                          \
                               .cur = &(vecptr)->cur,                          \
                               .elem_size = sizeof(type),                      \
                               .alloc = (vecptr)->alloc})
#define dbuf_init(vecptr, type, from, size)                                    \
  _dbuf_init((void **)from, size,                                              \
             (dbuf_data){.data_ptr = (void **)&(vecptr)->data,                 \
                         .len = &(vecptr)->len,                                \
                         .cur = &(vecptr)->cur,                                \
                         .elem_size = sizeof(type),                            \
                         .alloc = (vecptr)->alloc})

#define dbuf_make_alloc(vecptr, type, _size, _alloc)                           \
  _dbuf_make(_size, (dbuf_data){.data_ptr = (void **)&(vecptr)->data,          \
                                .len = &(vecptr)->len,                         \
                                .cur = &(vecptr)->cur,                         \
                                .elem_size = sizeof(type),                     \
                                .alloc = _alloc})
#define dbuf_init_alloc(vecptr, type, _from, _size, _alloc)                    \
  _dbuf_init((void **)_from, _size,                                            \
             (dbuf_data){.data_ptr = (void **)&(vecptr)->data,                 \
                         .len = &(vecptr)->len,                                \
                         .cur = &(vecptr)->cur,                                \
                         .elem_size = sizeof(type),                            \
                         .alloc = _alloc})
#define dbuf_push(vecptr, type, item)                                          \
  _dbuf_push(&(type){item}, (dbuf_data){.data_ptr = (void **)&(vecptr)->data,  \
                                        .len = &(vecptr)->len,                 \
                                        .cur = &(vecptr)->cur,                 \
                                        .elem_size = sizeof(type),             \
                                        .alloc = (vecptr)->alloc})
#define dbuf_pop(vecptr, type)                                                 \
  _dbuf_pop((dbuf_data){.data_ptr = (void **)&(vecptr)->data,                  \
                        .len = &(vecptr)->len,                                 \
                        .cur = &(vecptr)->cur,                                 \
                        .elem_size = sizeof(type),                             \
                        .alloc = (vecptr)->alloc})
#define dbuf_get(vecptr, type, id)                                             \
  _dbuf_get(id, (dbuf_data){.data_ptr = (void **)&(vecptr)->data,              \
                            .len = &(vecptr)->len,                             \
                            .cur = &(vecptr)->cur,                             \
                            .elem_size = sizeof(type),                         \
                            .alloc = (vecptr)->alloc})
#define dbuf_set(vecptr, type, id, item)                                       \
  _dbuf_set((void *)item, id,                                                  \
            (dbuf_data){.data_ptr = (void **)&(vecptr)->data,                  \
                        .len = &(vecptr)->len,                                 \
                        .cur = &(vecptr)->cur,                                 \
                        .elem_size = sizeof(type),                             \
                        .alloc = (vecptr)->alloc})
#define dbuf_insert(vecptr, type, item, id)                                    \
  _dbuf_insert((void *)&(type){item}, id,                                      \
               (dbuf_data){.data_ptr = (void **)&(vecptr)->data,               \
                           .len = &(vecptr)->len,                              \
                           .cur = &(vecptr)->cur,                              \
                           .elem_size = sizeof(type),                          \
                           .alloc = (vecptr)->alloc})
#define dbuf_remove(vecptr, type, id)                                          \
  _dbuf_remove(id, (dbuf_data){.data_ptr = (void **)&(vecptr)->data,           \
                               .len = &(vecptr)->len,                          \
                               .cur = &(vecptr)->cur,                          \
                               .elem_size = sizeof(type),                      \
                               .alloc = (vecptr)->alloc})

#define dbuf_append(vecptr, type, arr, len)                                    \
  _dbuf_append((void **)arr, len,                                              \
               (dbuf_data){.data_ptr = (void **)&(vecptr)->data,               \
                           .len = &(vecptr)->len,                              \
                           .cur = &(vecptr)->cur,                              \
                           .elem_size = sizeof(type),                          \
                           .alloc = (vecptr)->alloc})

#define dbuf_prepend(vecptr, type, arr, len)                                   \
  _dbuf_prepend((void **)arr, len,                                             \
                (dbuf_data){.data_ptr = (void **)&(vecptr)->data,              \
                            .len = &(vecptr)->len,                             \
                            .cur = &(vecptr)->cur,                             \
                            .elem_size = sizeof(type),                         \
                            .alloc = (vecptr)->alloc})

#define dbuf_reserve(vecptr, type, len)                                        \
  _dbuf_reserve(len, (dbuf_data){.data_ptr = (void **)&(vecptr)->data,         \
                                 .len = &(vecptr)->len,                        \
                                 .cur = &(vecptr)->cur,                        \
                                 .elem_size = sizeof(type),                    \
                                 .alloc = (vecptr)->alloc})

#define dbuf_free(vecptr)                                                      \
  _dbuf_free((dbuf_data){.data_ptr = (void **)&(vecptr)->data,                 \
                         .alloc = (vecptr)->alloc})

// === DSTR ===

// default `dbuf` for `dstr`s
dbuf_decl(dstr, dstr)

// convenience macro for C string literals
#define dstr(b) (dstr){ .cptr = (b), .len = sizeof((b)) - 1 }
// convenience macro for printf style formatting.
// use with `%.*s` when using printf
#define dstr_fmt(b) (int)(b).len, (b).cptr

// create new string, with an initial size of `init`
dstr dstr_new(isize init, d_allocator alloc);

// creates new string from a given c string
dstr dstr_from(char *from, d_allocator alloc);

// duplicates a string s and returns a copy
dstr dstr_dup (dstr s, d_allocator alloc);

// append d-string `src` to d-string `dest`
// i.e dstr_prepend("hello ", "world"), returns dstr("hello world")
dstr dstr_append      (dstr dest, dstr src, d_allocator alloc);

// append c string ( char * ) `from` to `to`
// i.e dstr_prepend_cstr("hello ", "world"), returns dstr("hello world")
dstr dstr_append_cstr (dstr to, const char * from, d_allocator alloc);

// prepend dstr `from` to `to`
// i.e dstr_prepend(" world", "hello"), returns dstr("hello world")
dstr dstr_prepend     (dstr to, dstr from, d_allocator alloc);

// prepend c string ( char * ) `from` to `to`
// i.e dstr_prepend_cstr(" world", "hello"), returns dstr("hello world")
dstr dstr_prepend_cstr(dstr to, const char * from, d_allocator alloc);

// checks if the given strins are equal in length
bool dstr_eq(dstr a, dstr b);

// checks if a given dstr `str` starts with a string `st`
bool dstr_starts_with(dstr str, const char* st);

void dstr_to_lower(dstr);
void dstr_to_upper(dstr);

// replace first instance of `pat` with `sub`
dstr dstr_replace     (dstr str, const char* pat, const char* sub, d_allocator alloc);

// replace all instances of `pat` with `sub`
// FIXME unimplemented
dstr dstr_replace_all (dstr str, const char* pat, const char* sub, d_allocator alloc);

// replace nth instance of `pat` with `sub`
// FIXME unimplemented
dstr dstr_replace_nth (dstr str, int n, const char* pat, const char* sub, d_allocator alloc);

// replace n instances of `pat` with `sub`
// FIXME unimplemented
dstr dstr_replace_n   (dstr str, int n, const char* pat, const char* sub, d_allocator alloc);

dbuf_dstr dstr_split_lines   (dstr src, d_allocator alloc);

dbuf_dstr dstr_split_n_lines (dstr src, isize n, d_allocator alloc); // FIXME: unimplemented

dbuf_dstr dstr_split_tokens  (dstr src, const char* tkn, d_allocator alloc);

int dstr_parse_int(dstr src);     // FIXME: Unimplemented

float dstr_parse_float(dstr src); // FIXME: Unimplemented

// === DFILE ===

u64 dfile_get_size(char* path);

dstr dfile_read           (char* file_path);
dstr dfile_read_bytes     (char* file_path, u64 bytes);
dbuf_dstr dfile_read_lines     (char* file_path);
// dbuf_dstr dfile_read_n_lines   (char* file_path, u64 num_lines); // TODO: Unimplemented

// int dfile_write           (char* file_path, dstr buf); // TODO: Unimplemented
// int dfile_write_bytes     (char* file_path, dstr buf, isize bytes); // TODO: Unimplemented
// int dfile_write_lines     (char* file_path, int num_lines, dbuf_dstr buf, u64* buf_len); // TODO: Unimplemented

dfilepath split_path(char* path);

// === MISC FUNCITONS ===

// ASCII only for now
bool d_char_is_digit    (char c);
bool d_char_is_alpha    (char c);
u8   d_char_to_digit    (char c);
bool d_char_is_alphanum (char c);

#ifdef __cplusplus
}
#endif
