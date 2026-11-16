#pragma once

#define _as_dbuf(DBUF) (dbuf){(DBUF).data, (DBUF).len, (DBUF).cur, .alloc = (DBUF).alloc}

#define __pop_dbuf_data(DATA, BUF, T) \
  (DATA).data_ptr = (void**)&(BUF).data; \
  (DATA).alloc = (BUF).alloc; \
  (DATA).cur = &(BUF).cur; \
  (DATA).len = &(BUF).len; \
  (DATA).elem_size = sizeof(T)

// this macro's been pulled into its own header for readability's sake
// i wanted to invoke the dbuf_decl macro for some of the types in `derp.h`
// but i didn't want to paste the entire macro into the middle of the file
// so instead, it lives here.
//
// be warned though that this macro depends on many of the types and macros defined
// in `derp.h`, so it must never be included on it's own.

// T is the base type, N is the name for the outputted datatype ( eg for when you have a char* vec, but you want to call it a string vec )
#define dbuf_decl(T, N)                                                        \
  typedef struct dbuf_##N {                                                    \
    isize cur, len;                                                            \
    T *data;                                                                   \
    d_allocator alloc;                                                         \
  } dbuf_##N;                                                                  \
                                                                               \
  static DATTR_UNUSED inline dbuf_##N dbuf_##N##_make(isize init) {            \
    dbuf_data buf = {0};                                                       \
    dbuf_##N d = {0};                                                          \
                                                                               \
    __pop_dbuf_data(buf, d, T);                                                \
    _dbuf_make(init, buf);                                                     \
    return d;                                                                  \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline dbuf_##N dbuf_##N##_init(T *data, isize init) {   \
    dbuf_data buf = {0};                                                       \
    dbuf_##N d = {0};                                                          \
                                                                               \
    __pop_dbuf_data(buf, d, T);                                                \
    _dbuf_init((const void **)&data, init, buf);                               \
    return d;                                                                  \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline dbuf_##N dbuf_##N##_make_alloc(                   \
      isize init, d_allocator alloc) {                                         \
    dbuf_data buf = {0};                                                       \
    dbuf_##N d = {0};                                                          \
                                                                               \
    d.alloc = alloc;                                                           \
                                                                               \
    __populate_dbuf_data(&buf, &_as_dbuf(d), sizeof(T));                       \
    __pop_dbuf_data(buf, d, T);                                                \
    _dbuf_make_alloc(init, alloc, buf);                                        \
    return d;                                                                  \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline dbuf_##N dbuf_##N##_init_alloc(                   \
      T *data, isize init, d_allocator alloc) {                                \
    dbuf_data buf = {0};                                                       \
    dbuf_##N d = {0};                                                          \
                                                                               \
    d.alloc = alloc;                                                           \
                                                                               \
    __pop_dbuf_data(buf, d, T);                                                \
    _dbuf_init_alloc((const void **)&data, init, alloc, buf);                  \
    return d;                                                                  \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_push(dbuf_##N *d, T i) {          \
    dbuf_data buf = {0};                                                       \
    __pop_dbuf_data(buf, *d, T);                                               \
    _dbuf_push(&i, buf);                                                       \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_insert(dbuf_##N *d, T i,          \
                                                    isize id) {                \
    dbuf_data buf = {0};                                                       \
    __pop_dbuf_data(buf, *d, T);                                               \
    _dbuf_insert(&i, id, buf);                                                 \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_remove(dbuf_##N *d, isize id) {   \
    dbuf_data buf = {0};                                                       \
    __pop_dbuf_data(buf, *d, T);                                               \
    _dbuf_remove(id, buf);                                                     \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline T dbuf_##N##_pop(dbuf_##N *d) {                   \
    dbuf_data buf = {0};                                                       \
    __pop_dbuf_data(buf, *d, T);                                               \
    T *x = (T *)_dbuf_pop(buf);                                                \
    return *x;                                                                 \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_append(dbuf_##N *d, T *arr,       \
                                                    isize len) {               \
    dbuf_data buf = {0};                                                       \
    __pop_dbuf_data(buf, *d, T);                                               \
    _dbuf_append((const void **)arr, len, buf);                                \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_prepend(dbuf_##N *d, T *arr,      \
                                                     isize len) {              \
    dbuf_data buf = {0};                                                       \
    __pop_dbuf_data(buf, *d, T);                                               \
    _dbuf_prepend((const void **)arr, len, buf);                               \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline T dbuf_##N##_get(dbuf_##N *d, isize id) {         \
    dbuf_data buf = {0};                                                       \
    __pop_dbuf_data(buf, *d, T);                                               \
    T *x = (T *)_dbuf_get(id, buf);                                            \
    return *x;                                                                 \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_set(dbuf_##N *d, isize id, T i) { \
    dbuf_data buf = {0};                                                       \
    __pop_dbuf_data(buf, *d, T);                                               \
    _dbuf_set(id, &i, buf);                                                    \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_reserve(dbuf_##N *d, isize len) { \
    dbuf_data buf = {0};                                                       \
    __pop_dbuf_data(buf, *d, T);                                               \
    _dbuf_reserve(len, buf);                                                   \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline dbuf_##N dbuf_##N##_clone(dbuf_##N src) {         \
    dbuf_##N dest = {0};                                                       \
    dbuf_data buf = {0};                                                       \
                                                                               \
    dest.alloc = src.alloc;                                                    \
                                                                               \
    __pop_dbuf_data(buf, dest, T);                                             \
    dbuf _src = (dbuf){.data = (const void **)src.data,                        \
                       .len = src.len,                                         \
                       .cur = src.cur,                                         \
                       .alloc = src.alloc,                                     \
                       .elem_size = sizeof(T)};                                \
    _dbuf_clone(buf, _src);                                                    \
    return dest;                                                               \
  }
