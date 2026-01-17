#pragma once

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
    dbuf_##N n = {0};                                                          \
                                                                               \
    buf.data = (void **)&n.data;                                               \
    buf.cur = &n.cur;                                                          \
    buf.len = &n.len;                                                          \
    buf.alloc = &n.alloc;                                                      \
    buf.elem_size = sizeof(T);                                                 \
    _dbuf_make(init, buf);                                                     \
    return n;                                                                  \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline dbuf_##N dbuf_##N##_init(T *data, isize init) {   \
    dbuf_data buf = {0};                                                       \
    dbuf_##N n = {0};                                                          \
                                                                               \
    buf.data = (void **)&n.data;                                               \
    buf.cur = &n.cur;                                                          \
    buf.len = &n.len;                                                          \
    buf.alloc = &n.alloc;                                                      \
    buf.elem_size = sizeof(T);                                                 \
    _dbuf_init((void **)&data, init, buf);                                     \
    return n;                                                                  \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline dbuf_##N dbuf_##N##_make_alloc(                   \
      isize init, d_allocator alloc) {                                         \
    dbuf_data buf = {0};                                                       \
    dbuf_##N n = {0};                                                          \
                                                                               \
    buf.data = (void **)&n.data;                                               \
    buf.cur = &n.cur;                                                          \
    buf.len = &n.len;                                                          \
    buf.alloc = &alloc;                                                        \
    buf.elem_size = sizeof(T);                                                 \
    _dbuf_make_alloc(init, alloc, buf);                                        \
    return n;                                                                  \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline dbuf_##N dbuf_##N##_init_alloc(                   \
      T *data, isize init, d_allocator alloc) {                                \
    dbuf_data buf = {0};                                                       \
    dbuf_##N n = {0};                                                          \
                                                                               \
    buf.data = (void **)&n.data;                                               \
    buf.cur = &n.cur;                                                          \
    buf.len = &n.len;                                                          \
    buf.alloc = &alloc;                                                        \
    buf.elem_size = sizeof(T);                                                 \
    _dbuf_init_alloc((void **)&data, init, alloc, buf);                        \
    return n;                                                                  \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_push(dbuf_##N *d, T i) {          \
    dbuf_data buf = {0};                                                       \
    buf.data = (void **)&d->data;                                              \
    buf.cur = &d->cur;                                                         \
    buf.len = &d->len;                                                         \
    buf.elem_size = sizeof(T);                                                 \
    buf.alloc = &d->alloc;                                                     \
    _dbuf_push(&i, buf);                                                       \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_insert(dbuf_##N *d, T i,          \
                                                    isize id) {                \
    dbuf_data buf = {0};                                                       \
    buf.data = (void **)&d->data;                                              \
    buf.cur = &d->cur;                                                         \
    buf.len = &d->len;                                                         \
    buf.elem_size = sizeof(T);                                                 \
    buf.alloc = &d->alloc;                                                     \
    _dbuf_insert(&i, id, buf);                                                 \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_remove(dbuf_##N *d, isize id) {   \
    dbuf_data buf = {0};                                                       \
    buf.data = (void **)&d->data;                                              \
    buf.cur = &d->cur;                                                         \
    buf.len = &d->len;                                                         \
    buf.elem_size = sizeof(T);                                                 \
    buf.alloc = &d->alloc;                                                     \
    _dbuf_remove(id, buf);                                                     \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline T dbuf_##N##_pop(dbuf_##N *d) {                   \
    dbuf_data buf = {0};                                                       \
    buf.data = (void **)&d->data;                                              \
    buf.cur = &d->cur;                                                         \
    buf.len = &d->len;                                                         \
    buf.elem_size = sizeof(T);                                                 \
    buf.alloc = &d->alloc;                                                     \
    T *x = (T *)_dbuf_pop(buf);                                                \
    return *x;                                                                 \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_append(dbuf_##N *d, T *arr,       \
                                                    isize len) {               \
    dbuf_data buf = {0};                                                       \
    buf.data = (void **)&d->data;                                              \
    buf.cur = &d->cur;                                                         \
    buf.len = &d->len;                                                         \
    buf.elem_size = sizeof(T);                                                 \
    buf.alloc = &d->alloc;                                                     \
    _dbuf_append((void **)arr, len, buf);                                      \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_prepend(dbuf_##N *d, T *arr,      \
                                                     isize len) {              \
    dbuf_data buf = {0};                                                       \
    buf.data = (void **)&d->data;                                              \
    buf.cur = &d->cur;                                                         \
    buf.len = &d->len;                                                         \
    buf.elem_size = sizeof(T);                                                 \
    buf.alloc = &d->alloc;                                                     \
    _dbuf_prepend((void **)arr, len, buf);                                     \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline T dbuf_##N##_get(dbuf_##N *d, isize id) {         \
    dbuf_data buf = {0};                                                       \
    buf.data = (void **)&d->data;                                              \
    buf.cur = &d->cur;                                                         \
    buf.len = &d->len;                                                         \
    buf.elem_size = sizeof(T);                                                 \
    buf.alloc = &d->alloc;                                                     \
    T *x = (T *)_dbuf_get(id, buf);                                            \
    return *x;                                                                 \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_set(dbuf_##N *d, isize id, T i) { \
    dbuf_data buf = {0};                                                       \
    buf.data = (void **)&d->data;                                              \
    buf.cur = &d->cur;                                                         \
    buf.len = &d->len;                                                         \
    buf.elem_size = sizeof(T);                                                 \
    buf.alloc = &d->alloc;                                                     \
    _dbuf_set(id, &i, buf);                                                    \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline void dbuf_##N##_reserve(dbuf_##N *d, isize len) { \
    dbuf_data buf = {0};                                                       \
    buf.data = (void **)&d->data;                                              \
    buf.cur = &d->cur;                                                         \
    buf.len = &d->len;                                                         \
    buf.elem_size = sizeof(T);                                                 \
    buf.alloc = &d->alloc;                                                     \
    _dbuf_reserve(len, buf);                                                   \
  }                                                                            \
                                                                               \
  static DATTR_UNUSED inline dbuf_##N dbuf_##N##_clone(dbuf_##N src) {         \
    dbuf_##N dest = {0};                                                       \
    dbuf_data buf = {0};                                                       \
    buf.data = (void **)&dest.data;                                            \
    buf.cur = &dest.cur;                                                       \
    buf.len = &dest.len;                                                       \
    buf.elem_size = sizeof(T);                                                 \
    buf.alloc = &dest.alloc;                                                   \
    dbuf _src = (dbuf){.data = (void **)src.data,                              \
                       .len = src.len,                                         \
                       .cur = src.cur,                                         \
                       .alloc = &src.alloc,                                    \
                       .elem_size = sizeof(T)};                                \
    _dbuf_clone(buf, _src);                                                    \
    return dest;                                                               \
  }\
