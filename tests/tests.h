#pragma once
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include "../derp.h"

#include <stdbool.h>
#include <stddef.h>

#define TEST_ARRAY_LEN(array) (sizeof(array) / sizeof((array)[0]))
#define TEST_CASE(function) {#function, function}
#define TEST_TEMP_PATH_SIZE 64

typedef bool (*test_function)(void);

typedef struct test_case {
  const char *name;
  test_function run;
} test_case;

void test_fail(const char *file, int line, const char *fmt, ...);
bool test_dstr_check(const char *expected, dstr actual, const char *file, int line);
void test_free_dstr(dstr *string, d_allocator alloc);
void test_free_dbuf_dstr(dbuf_dstr *strings, d_allocator alloc);
bool test_make_temp_file(const void *contents, size_t length,
                         char path[TEST_TEMP_PATH_SIZE]);
void test_remove_temp_file(char path[TEST_TEMP_PATH_SIZE]);

#define TEST_ASSERT(condition)                                                  \
  do {                                                                          \
    if (!(condition)) {                                                         \
      test_fail(__FILE__, __LINE__, "assertion failed: %s", #condition);       \
      return false;                                                             \
    }                                                                           \
  } while (0)

#define TEST_ASSERT_DSTR(expected, actual)                                      \
  do {                                                                          \
    if (!test_dstr_check((expected), (actual), __FILE__, __LINE__)) {           \
      return false;                                                             \
    }                                                                           \
  } while (0)

extern const test_case tests_dmem[];
extern const size_t tests_dmem_count;
extern const test_case tests_ddebug[];
extern const size_t tests_ddebug_count;
extern const test_case tests_darena[];
extern const size_t tests_darena_count;
extern const test_case tests_dbuf[];
extern const size_t tests_dbuf_count;
extern const test_case tests_dstr[];
extern const size_t tests_dstr_count;
extern const test_case tests_dfile[];
extern const size_t tests_dfile_count;
