#include "tests.h"

#include <string.h>

static const unsigned char binary_contents[] = {
    'a', 'b', '\n', 'c', '\0', 'd', '\n', 'e', 'f',
};

static const char text_contents[] = "one\ntwo\n\nfour\n";

static bool test_size_and_whole_file_read(void) {
  char path[TEST_TEMP_PATH_SIZE] = {0};
  if (!test_make_temp_file(binary_contents, sizeof(binary_contents), path)) {
    return false;
  }
  u64 size = dfile_get_size(path);
  dstr contents = dfile_read(path);
  test_remove_temp_file(path);

  TEST_ASSERT(size == sizeof(binary_contents));
  TEST_ASSERT(contents.len == (isize)sizeof(binary_contents));
  TEST_ASSERT(contents.cptr != NULL);
  TEST_ASSERT(memcmp(contents.cptr, binary_contents, sizeof(binary_contents)) == 0);
  test_free_dstr(&contents, def_allocator);
  return true;
}

static bool test_read_bytes_returns_only_the_requested_prefix(void) {
  char path[TEST_TEMP_PATH_SIZE] = {0};
  if (!test_make_temp_file(binary_contents, sizeof(binary_contents), path)) {
    return false;
  }
  dstr prefix = dfile_read_bytes(path, 4);
  dstr explicit_prefix = dfile_read_bytes_alloc(path, 5, def_allocator);
  test_remove_temp_file(path);

  TEST_ASSERT(prefix.len == 4);
  TEST_ASSERT(memcmp(prefix.cptr, binary_contents, 4) == 0);
  TEST_ASSERT(explicit_prefix.len == 5);
  TEST_ASSERT(memcmp(explicit_prefix.cptr, binary_contents, 5) == 0);
  test_free_dstr(&prefix, def_allocator);
  test_free_dstr(&explicit_prefix, def_allocator);
  return true;
}

static bool test_read_lines_with_default_and_explicit_allocators(void) {
  char path[TEST_TEMP_PATH_SIZE] = {0};
  if (!test_make_temp_file(text_contents, strlen(text_contents), path)) {
    return false;
  }
  dbuf_dstr lines = dfile_read_lines(path);
  dbuf_dstr allocated_lines = dfile_read_lines_alloc(path, def_allocator);
  test_remove_temp_file(path);

  TEST_ASSERT(lines.cur == 3);
  TEST_ASSERT_DSTR("one", lines.data[0]);
  TEST_ASSERT_DSTR("two", lines.data[1]);
  TEST_ASSERT_DSTR("four", lines.data[2]);
  TEST_ASSERT(allocated_lines.cur == 3);
  TEST_ASSERT_DSTR("one", allocated_lines.data[0]);
  TEST_ASSERT_DSTR("two", allocated_lines.data[1]);
  TEST_ASSERT_DSTR("four", allocated_lines.data[2]);
  test_free_dbuf_dstr(&lines, def_allocator);
  test_free_dbuf_dstr(&allocated_lines, def_allocator);
  return true;
}

static bool test_split_path_extracts_path_and_extension(void) {
  char path[] = "dir/archive.txt";
  dfilepath parts = split_path(path);
  TEST_ASSERT_DSTR(path, parts.full_path);
  TEST_ASSERT(parts.full_path.cptr == path);
  TEST_ASSERT_DSTR("dir/archive", parts.path);
  TEST_ASSERT_DSTR(".txt", parts.ext);
  d_free(parts.path.cptr);
  d_free(parts.ext.cptr);
  return true;
}

static bool test_empty_file_has_zero_size_and_length(void) {
  char path[TEST_TEMP_PATH_SIZE] = {0};
  if (!test_make_temp_file("", 0, path)) {
    return false;
  }
  u64 size = dfile_get_size(path);
  dstr contents = dfile_read(path);
  test_remove_temp_file(path);

  TEST_ASSERT(size == 0);
  TEST_ASSERT(contents.len == 0);
  TEST_ASSERT(contents.cptr != NULL);
  test_free_dstr(&contents, def_allocator);
  return true;
}

const test_case tests_dfile[] = {
    TEST_CASE(test_size_and_whole_file_read),
    TEST_CASE(test_read_bytes_returns_only_the_requested_prefix),
    TEST_CASE(test_read_lines_with_default_and_explicit_allocators),
    TEST_CASE(test_split_path_extracts_path_and_extension),
    TEST_CASE(test_empty_file_has_zero_size_and_length),
};
const size_t tests_dfile_count = TEST_ARRAY_LEN(tests_dfile);
