#include "tests.h"

#include <stdlib.h>
#include <string.h>

typedef struct tracked_memory {
  int allocations;
  int reallocations;
  int frees;
  isize last_size;
  bool last_clean;
  const char *last_file;
  int last_line;
  bool context_matched;
} tracked_memory;

static void *tracked_alloc(isize len, bool clean, d_alloc_data data) {
  tracked_memory *tracker = data.ctx;
  tracker->allocations++;
  tracker->last_size = len;
  tracker->last_clean = clean;
  tracker->last_file = data.file;
  tracker->last_line = data.line_num;
  tracker->context_matched = data.ctx == tracker;
  return clean ? calloc(1, (size_t)len) : malloc((size_t)len);
}

static void *tracked_realloc(void *block, isize len, d_alloc_data data) {
  tracked_memory *tracker = data.ctx;
  tracker->reallocations++;
  tracker->last_size = len;
  tracker->last_file = data.file;
  tracker->last_line = data.line_num;
  tracker->context_matched = data.ctx == tracker;
  return realloc(block, (size_t)len);
}

static void tracked_free(void *pointer, d_alloc_data data) {
  tracked_memory *tracker = data.ctx;
  tracker->frees++;
  tracker->last_file = data.file;
  tracker->last_line = data.line_num;
  tracker->context_matched = data.ctx == tracker;
  free(pointer);
}

static d_allocator tracked_allocator(tracked_memory *tracker) {
  return (d_allocator){
      .alloc = tracked_alloc,
      .realloc = tracked_realloc,
      .free = tracked_free,
      .ctx = tracker,
  };
}

static bool test_default_allocation_and_reallocation(void) {
  unsigned char *bytes = d_alloc(8);
  TEST_ASSERT(bytes != NULL);
  for (unsigned char index = 0; index < 8; index++) {
    bytes[index] = index;
  }

  unsigned char *grown = d_realloc(bytes, 16);
  TEST_ASSERT(grown != NULL);
  for (unsigned char index = 0; index < 8; index++) {
    TEST_ASSERT(grown[index] == index);
  }
  d_free(grown);

  int *zeros = d_calloc(4, sizeof(*zeros));
  TEST_ASSERT(zeros != NULL);
  for (int index = 0; index < 4; index++) {
    TEST_ASSERT(zeros[index] == 0);
  }
  d_free(zeros);
  return true;
}

static bool test_set_default_allocator(void) {
  d_allocator original = def_allocator;
  tracked_memory tracker = {0};
  d_allocator custom = tracked_allocator(&tracker);
  dalloc_set_default(custom);

  unsigned char *bytes = d_alloc(8);
  int *zeros = d_calloc(4, sizeof(*zeros));
  bool calloc_cleared = zeros != NULL;
  if (zeros != NULL) {
    for (int index = 0; index < 4; index++) {
      calloc_cleared = calloc_cleared && zeros[index] == 0;
    }
  }
  unsigned char *grown = d_realloc(bytes, 16);
  d_free(grown);
  d_free(zeros);
  dalloc_set_default(original);

  TEST_ASSERT(grown != NULL);
  TEST_ASSERT(calloc_cleared);
  TEST_ASSERT(tracker.allocations == 2);
  TEST_ASSERT(tracker.reallocations == 1);
  TEST_ASSERT(tracker.frees == 2);
  TEST_ASSERT(tracker.context_matched);
  TEST_ASSERT(tracker.last_file != NULL);
  TEST_ASSERT(tracker.last_line > 0);
  return true;
}

static bool test_explicit_allocator_and_calloc(void) {
  tracked_memory tracker = {0};
  d_allocator custom = tracked_allocator(&tracker);

  int *values = dalloc_calloc(custom, 3, sizeof(*values));
  TEST_ASSERT(values != NULL);
  TEST_ASSERT(tracker.allocations == 1);
  TEST_ASSERT(tracker.last_size == (isize)(3 * sizeof(*values)));
  TEST_ASSERT(tracker.last_clean);
  TEST_ASSERT(values[0] == 0 && values[1] == 0 && values[2] == 0);

  values[0] = 42;
  int *resized = dalloc_realloc(custom, values, 6 * sizeof(*values));
  TEST_ASSERT(resized != NULL);
  TEST_ASSERT(resized[0] == 42);
  TEST_ASSERT(tracker.reallocations == 1);
  TEST_ASSERT(tracker.last_size == (isize)(6 * sizeof(*values)));

  dalloc_free(custom, resized);
  TEST_ASSERT(tracker.frees == 1);
  TEST_ASSERT(tracker.context_matched);
  return true;
}

const test_case tests_dmem[] = {
    TEST_CASE(test_default_allocation_and_reallocation),
    TEST_CASE(test_set_default_allocator),
    TEST_CASE(test_explicit_allocator_and_calloc),
};
const size_t tests_dmem_count = TEST_ARRAY_LEN(tests_dmem);
