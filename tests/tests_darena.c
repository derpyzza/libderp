#include "tests.h"

#include <stdint.h>
#include <string.h>

static bool test_initialization_and_separate_allocations(void) {
  d_arena arena = darena_init(4, def_allocator);
  TEST_ASSERT(arena.head != NULL);
  TEST_ASSERT(arena.head == arena.tail);
  TEST_ASSERT(arena.bucket_size == 4);
  TEST_ASSERT(arena.allocations == 0);

  unsigned char *first = darena_alloc(&arena, 3);
  int *second = darena_alloc(&arena, sizeof(*second));
  TEST_ASSERT(first != NULL);
  TEST_ASSERT(second != NULL);
  TEST_ASSERT((void *)first != (void *)second);
  TEST_ASSERT((uintptr_t)first % _Alignof(isize) == 0);
  TEST_ASSERT((uintptr_t)second % _Alignof(isize) == 0);
  memset(first, 0xab, 3);
  *second = 1234;
  TEST_ASSERT(first[0] == 0xab && first[2] == 0xab);
  TEST_ASSERT(*second == 1234);
  TEST_ASSERT(arena.allocations == 2);
  darena_free(&arena);
  return true;
}

static bool test_growth_clear_and_reuse(void) {
  d_arena arena = darena_init(2, def_allocator);
  isize *first = darena_alloc(&arena, sizeof(*first));
  isize *second = darena_alloc(&arena, sizeof(*second));
  isize *third = darena_alloc(&arena, sizeof(*third));
  TEST_ASSERT(first != NULL && second != NULL && third != NULL);
  TEST_ASSERT(arena.head != arena.tail);
  TEST_ASSERT(arena.head->next != NULL);
  *first = 10;
  *second = 20;
  *third = 30;
  TEST_ASSERT(*first == 10 && *second == 20 && *third == 30);

  darena_clear(&arena);
  TEST_ASSERT(arena.tail == arena.head);
  TEST_ASSERT(arena.head->index == 0);
  TEST_ASSERT(arena.head->next->index == 0);
  isize *reused = darena_alloc(&arena, sizeof(*reused));
  TEST_ASSERT(reused == first);
  *reused = 99;
  TEST_ASSERT(*first == 99);

  darena_free(&arena);
  TEST_ASSERT(arena.head == NULL);
  TEST_ASSERT(arena.tail == NULL);
  return true;
}

static bool test_arena_allocator_zeroes_clean_allocations(void) {
  d_arena arena = darena_init(4, def_allocator);
  d_allocator allocator = darena_make_allocator(&arena);
  TEST_ASSERT(allocator.alloc != NULL);
  TEST_ASSERT(allocator.realloc != NULL);
  TEST_ASSERT(allocator.free != NULL);
  TEST_ASSERT(allocator.ctx == &arena);

  unsigned char *initial = dalloc_alloc(allocator, 16);
  TEST_ASSERT(initial != NULL);
  memset(initial, 0xff, 16);
  darena_clear(&arena);
  unsigned char *cleared = dalloc_calloc(allocator, 16, sizeof(*cleared));
  TEST_ASSERT(cleared == initial);
  for (int index = 0; index < 16; index++) {
    TEST_ASSERT(cleared[index] == 0);
  }
  darena_free(&arena);
  return true;
}

static bool test_negative_allocation_is_rejected(void) {
  d_arena arena = darena_init(2, def_allocator);
  TEST_ASSERT(darena_alloc(&arena, -1) == NULL);
  TEST_ASSERT(arena.allocations == 0);
  darena_free(&arena);
  return true;
}

const test_case tests_darena[] = {
    TEST_CASE(test_initialization_and_separate_allocations),
    TEST_CASE(test_growth_clear_and_reuse),
    TEST_CASE(test_arena_allocator_zeroes_clean_allocations),
    TEST_CASE(test_negative_allocation_is_rejected),
};
const size_t tests_darena_count = TEST_ARRAY_LEN(tests_darena);
