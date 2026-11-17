#include "tests.h"

dbuf_decl(int, ints)

static bool test_make_zeroes_capacity(void) {
  dbuf_ints values = dbuf_ints_make(3);
  TEST_ASSERT(values.data != NULL);
  TEST_ASSERT(values.cur == 0);
  TEST_ASSERT(values.len == 3);
  TEST_ASSERT(values.alloc.free != NULL);
  for (int index = 0; index < 3; index++) {
    TEST_ASSERT(values.data[index] == 0);
  }
  dbuf_free(&values);
  return true;
}

static bool test_init_copies_source_array(void) {
  int source[] = {10, 20, 30};
  dbuf_ints values = dbuf_ints_init(source, 3);
  TEST_ASSERT(values.data != NULL);
  TEST_ASSERT(values.data != source);
  TEST_ASSERT(values.cur == 3);
  TEST_ASSERT(values.len == 3);
  source[0] = 99;
  TEST_ASSERT(values.data[0] == 10);
  TEST_ASSERT(values.data[1] == 20);
  TEST_ASSERT(values.data[2] == 30);
  dbuf_free(&values);
  return true;
}

static bool test_reserve_and_push_grow_without_losing_values(void) {
  dbuf_ints values = dbuf_ints_make(1);
  dbuf_ints_push(&values, 11);
  dbuf_ints_push(&values, 22);
  TEST_ASSERT(values.cur == 2);
  TEST_ASSERT(values.len >= 2);
  dbuf_ints_push(&values, 33);
  TEST_ASSERT(values.cur == 3);
  TEST_ASSERT(values.len >= 3);
  TEST_ASSERT(values.data[0] == 11);
  TEST_ASSERT(values.data[1] == 22);
  TEST_ASSERT(values.data[2] == 33);

  isize old_capacity = values.len;
  dbuf_ints_reserve(&values, 5);
  TEST_ASSERT(values.len == old_capacity + 5);
  TEST_ASSERT(values.cur == 3);
  TEST_ASSERT(values.data[2] == 33);
  dbuf_free(&values);
  return true;
}

static bool test_insert_get_and_set(void) {
  int source[] = {10, 20, 30};
  dbuf_ints values = dbuf_ints_init(source, 3);
  dbuf_ints_insert(&values, 40, 3);
  dbuf_ints_insert(&values, 5, 0);
  dbuf_ints_insert(&values, 25, 3);
  int expected[] = {5, 10, 20, 25, 30, 40};
  TEST_ASSERT(values.cur == 6);
  for (isize index = 0; index < values.cur; index++) {
    TEST_ASSERT(dbuf_ints_get(&values, index) == expected[index]);
  }
  dbuf_ints_set(&values, 3, 26);
  TEST_ASSERT(values.data[3] == 26);
  dbuf_free(&values);
  return true;
}

static bool test_remove_and_pop(void) {
  int source[] = {1, 2, 3, 4};
  dbuf_ints values = dbuf_ints_init(source, 4);
  dbuf_ints_remove(&values, 1);
  TEST_ASSERT(values.cur == 3);
  TEST_ASSERT(values.data[0] == 1);
  TEST_ASSERT(values.data[1] == 3);
  TEST_ASSERT(values.data[2] == 4);
  dbuf_ints_remove(&values, 2);
  TEST_ASSERT(values.cur == 2);
  TEST_ASSERT(dbuf_ints_pop(&values) == 3);
  TEST_ASSERT(values.cur == 1);
  TEST_ASSERT(values.data[0] == 1);
  dbuf_free(&values);
  return true;
}

static bool test_append_and_prepend_preserve_order(void) {
  int source[] = {1, 2};
  int suffix[] = {3, 4, 5};
  int prefix[] = {-1, 0};
  dbuf_ints values = dbuf_ints_init(source, 2);
  dbuf_ints_append(&values, suffix, 3);
  TEST_ASSERT(values.cur == 5);
  TEST_ASSERT(values.len >= values.cur);
  dbuf_ints_prepend(&values, prefix, 2);
  TEST_ASSERT(values.cur == 7);
  TEST_ASSERT(values.len >= values.cur);
  int expected[] = {-1, 0, 1, 2, 3, 4, 5};
  for (isize index = 0; index < values.cur; index++) {
    TEST_ASSERT(values.data[index] == expected[index]);
  }
  dbuf_free(&values);
  return true;
}

static bool test_clone_is_an_independent_copy(void) {
  dbuf_ints original = dbuf_ints_make(4);
  dbuf_ints_push(&original, 7);
  dbuf_ints_push(&original, 8);
  dbuf_ints clone = dbuf_ints_clone(original);
  TEST_ASSERT(clone.data != NULL);
  TEST_ASSERT(clone.data != original.data);
  TEST_ASSERT(clone.cur == original.cur);
  TEST_ASSERT(clone.len == original.len);
  clone.data[0] = 70;
  TEST_ASSERT(original.data[0] == 7);
  TEST_ASSERT(clone.data[1] == 8);
  dbuf_free(&clone);
  dbuf_free(&original);
  return true;
}

static bool test_iteration_visits_each_element(void) {
  int source[] = {2, 4, 6, 8};
  dbuf_ints values = dbuf_ints_init(source, 4);
  int sum = 0;
  dbuf_foreach(int, value, values) { sum += *value; }
  TEST_ASSERT(sum == 20);

  int index_sum = 0;
  dbuf_foreach_id(int, value, index, values) {
    index_sum += index;
    TEST_ASSERT(*value == source[index]);
  }
  TEST_ASSERT(index_sum == 6);
  dbuf_free(&values);
  return true;
}

static bool test_generic_macros(void) {
  dbuf values = {.alloc = def_allocator};
  dbuf_make(&values, int, 2);
  dbuf_push(&values, int, 4);
  dbuf_push(&values, int, 8);
  dbuf_set(&values, int, 1, 9);
  TEST_ASSERT(*(int *)dbuf_get(&values, int, 1) == 9);
  dbuf_insert(&values, int, 7, 1);
  TEST_ASSERT(values.cur == 3);
  TEST_ASSERT(((int *)values.data)[0] == 4);
  TEST_ASSERT(((int *)values.data)[1] == 7);
  TEST_ASSERT(((int *)values.data)[2] == 9);
  dbuf_remove(&values, int, 0);
  TEST_ASSERT(values.cur == 2);
  TEST_ASSERT(*(int *)dbuf_pop(&values, int) == 9);
  TEST_ASSERT(values.cur == 1);
  dbuf_free(&values);

  int source[] = {15, 16};
  dbuf initialized = {.alloc = def_allocator};
  dbuf_init(&initialized, int, source, 2);
  TEST_ASSERT(initialized.cur == 2);
  TEST_ASSERT(((int *)initialized.data)[0] == 15);
  TEST_ASSERT(((int *)initialized.data)[1] == 16);
  dbuf_free(&initialized);
  return true;
}

const test_case tests_dbuf[] = {
    TEST_CASE(test_make_zeroes_capacity),
    TEST_CASE(test_init_copies_source_array),
    TEST_CASE(test_reserve_and_push_grow_without_losing_values),
    TEST_CASE(test_insert_get_and_set),
    TEST_CASE(test_remove_and_pop),
    TEST_CASE(test_append_and_prepend_preserve_order),
    TEST_CASE(test_clone_is_an_independent_copy),
    TEST_CASE(test_iteration_visits_each_element),
    TEST_CASE(test_generic_macros),
};
const size_t tests_dbuf_count = TEST_ARRAY_LEN(tests_dbuf);
