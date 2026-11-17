#include "tests.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void test_fail(const char *file, int line, const char *fmt, ...) {
  va_list args;
  fprintf(stderr, "    %s:%d: ", file, line);
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fputc('\n', stderr);
}

bool test_dstr_check(const char *expected, dstr actual, const char *file,
                     int line) {
  size_t expected_len = strlen(expected);
  if (actual.len == (isize)expected_len && actual.cptr != NULL &&
      memcmp(actual.cptr, expected, expected_len) == 0) {
    return true;
  }

  test_fail(file, line, "expected string \"%s\", got \"%.*s\" (length %td)",
            expected, actual.cptr == NULL ? 0 : (int)actual.len,
            actual.cptr == NULL ? "" : actual.cptr, actual.len);
  return false;
}

void test_free_dstr(dstr *string, d_allocator alloc) {
  if (string->cptr != NULL) {
    dalloc_free(alloc, string->cptr);
  }
  *string = (dstr){0};
}

void test_free_dbuf_dstr(dbuf_dstr *strings, d_allocator alloc) {
  for (isize index = 0; index < strings->cur; index++) {
    test_free_dstr(&strings->data[index], alloc);
  }
  if (strings->data != NULL) {
    dalloc_free(alloc, strings->data);
  }
  *strings = (dbuf_dstr){0};
}

bool test_make_temp_file(const void *contents, size_t length,
                         char path[TEST_TEMP_PATH_SIZE]) {
  snprintf(path, TEST_TEMP_PATH_SIZE, "/tmp/libderp-tests-XXXXXX");
  int descriptor = mkstemp(path);
  if (descriptor < 0) {
    test_fail(__FILE__, __LINE__, "mkstemp failed: %s", strerror(errno));
    return false;
  }

  const char *bytes = contents;
  size_t written = 0;
  while (written < length) {
    ssize_t result = write(descriptor, bytes + written, length - written);
    if (result < 0 && errno == EINTR) {
      continue;
    }
    if (result <= 0) {
      test_fail(__FILE__, __LINE__, "writing temporary file failed: %s",
                strerror(errno));
      close(descriptor);
      test_remove_temp_file(path);
      return false;
    }
    written += (size_t)result;
  }

  if (close(descriptor) != 0) {
    test_fail(__FILE__, __LINE__, "closing temporary file failed: %s",
              strerror(errno));
    test_remove_temp_file(path);
    return false;
  }
  return true;
}

void test_remove_temp_file(char path[TEST_TEMP_PATH_SIZE]) {
  if (path[0] != '\0') {
    unlink(path);
    path[0] = '\0';
  }
}
