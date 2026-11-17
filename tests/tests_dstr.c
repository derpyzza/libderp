#include "tests.h"

#include <string.h>

static bool test_new_from_and_duplicate(void) {
  dstr allocated = dstr_new(4, def_allocator);
  TEST_ASSERT(allocated.cptr != NULL);
  TEST_ASSERT(allocated.len == 4);
  memcpy(allocated.cptr, "abcd", 4);

  char source[] = "Hello";
  dstr copied = dstr_from(source, def_allocator);
  TEST_ASSERT_DSTR("Hello", copied);
  TEST_ASSERT(copied.cptr != source);
  source[0] = 'J';
  TEST_ASSERT_DSTR("Hello", copied);

  dstr duplicate = dstr_dup(copied, def_allocator);
  TEST_ASSERT_DSTR("Hello", duplicate);
  TEST_ASSERT(duplicate.cptr != copied.cptr);
  duplicate.cptr[0] = 'Y';
  TEST_ASSERT(memcmp(copied.cptr, "Hello", 5) == 0);

  test_free_dstr(&allocated, def_allocator);
  test_free_dstr(&copied, def_allocator);
  test_free_dstr(&duplicate, def_allocator);
  return true;
}

static bool test_append_strings(void) {
  dstr joined = dstr_append(dstr("hello "), dstr("world"), def_allocator);
  TEST_ASSERT_DSTR("hello world", joined);
  test_free_dstr(&joined, def_allocator);

  dstr suffix = dstr_append_cstr(dstr("hello"), " world", def_allocator);
  TEST_ASSERT_DSTR("hello world", suffix);
  test_free_dstr(&suffix, def_allocator);
  return true;
}

static bool test_prepend_strings(void) {
  dstr joined = dstr_prepend(dstr("world"), dstr("hello "), def_allocator);
  TEST_ASSERT_DSTR("hello world", joined);
  test_free_dstr(&joined, def_allocator);

  dstr prefix = dstr_prepend_cstr(dstr("world"), "hello ", def_allocator);
  TEST_ASSERT_DSTR("hello world", prefix);
  test_free_dstr(&prefix, def_allocator);
  return true;
}

static bool test_equality_uses_length_and_contents(void) {
  TEST_ASSERT(dstr_eq(dstr("same"), dstr("same")));
  TEST_ASSERT(!dstr_eq(dstr("same"), dstr("sand")));
  TEST_ASSERT(!dstr_eq(dstr("same"), dstr("same!")));

  char left[] = {'a', '\0', 'b'};
  char right[] = {'a', '\0', 'b'};
  dstr left_view = {.cptr = left, .len = 3};
  dstr right_view = {.cptr = right, .len = 3};
  TEST_ASSERT(dstr_eq(left_view, right_view));
  right[2] = 'c';
  TEST_ASSERT(!dstr_eq(left_view, right_view));
  return true;
}

static bool test_case_conversion_changes_only_alphabetic_characters(void) {
  char text[] = "Ab-Cd 9!";
  dstr view = {.cptr = text, .len = (isize)strlen(text)};
  dstr_to_lower(view);
  TEST_ASSERT(strcmp(text, "ab-cd 9!") == 0);
  dstr_to_upper(view);
  TEST_ASSERT(strcmp(text, "AB-CD 9!") == 0);
  return true;
}

static bool test_split_tokens_reads_a_bounded_view(void) {
  char text[] = {'a', ',', 'b', ',', ',', 'c'};
  dstr view = {.cptr = text, .len = 6};
  dbuf_dstr tokens = dstr_split_tokens(view, ",", def_allocator);
  TEST_ASSERT(tokens.cur == 3);
  TEST_ASSERT_DSTR("a", tokens.data[0]);
  TEST_ASSERT_DSTR("b", tokens.data[1]);
  TEST_ASSERT_DSTR("c", tokens.data[2]);
  test_free_dbuf_dstr(&tokens, def_allocator);
  return true;
}

static bool test_split_lines(void) {
  dbuf_dstr lines = dstr_split_lines(dstr("first\nsecond\nthird"), def_allocator);
  TEST_ASSERT(lines.cur == 3);
  TEST_ASSERT_DSTR("first", lines.data[0]);
  TEST_ASSERT_DSTR("second", lines.data[1]);
  TEST_ASSERT_DSTR("third", lines.data[2]);
  test_free_dbuf_dstr(&lines, def_allocator);
  return true;
}

static bool test_ascii_character_helpers(void) {
  TEST_ASSERT(d_char_is_digit('0'));
  TEST_ASSERT(d_char_is_digit('9'));
  TEST_ASSERT(!d_char_is_digit('a'));
  TEST_ASSERT(d_char_to_digit('7') == 7);
  TEST_ASSERT(d_char_is_alpha('A'));
  TEST_ASSERT(d_char_is_alpha('z'));
  TEST_ASSERT(!d_char_is_alpha('4'));
  TEST_ASSERT(d_char_is_alphanum('x'));
  TEST_ASSERT(d_char_is_alphanum('5'));
  TEST_ASSERT(!d_char_is_alphanum('_'));
  return true;
}

const test_case tests_dstr[] = {
    TEST_CASE(test_new_from_and_duplicate),
    TEST_CASE(test_append_strings),
    TEST_CASE(test_prepend_strings),
    TEST_CASE(test_equality_uses_length_and_contents),
    TEST_CASE(test_case_conversion_changes_only_alphabetic_characters),
    TEST_CASE(test_split_tokens_reads_a_bounded_view),
    TEST_CASE(test_split_lines),
    TEST_CASE(test_ascii_character_helpers),
};
const size_t tests_dstr_count = TEST_ARRAY_LEN(tests_dstr);
