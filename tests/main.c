#include "tests.h"

#include <stdio.h>

typedef struct test_suite {
  const char *name;
  const test_case *cases;
  size_t count;
} test_suite;

int main(void) {
  const test_suite suites[] = {
      {"dmem", tests_dmem, tests_dmem_count},
      {"ddebug", tests_ddebug, tests_ddebug_count},
      {"darena", tests_darena, tests_darena_count},
      {"dbuf", tests_dbuf, tests_dbuf_count},
      {"dstr", tests_dstr, tests_dstr_count},
      {"dfile", tests_dfile, tests_dfile_count},
  };
  size_t passed = 0;
  size_t failed = 0;
  size_t total = 0;

  printf("libderp test suite\n");
  for (size_t suite_index = 0;
       suite_index < TEST_ARRAY_LEN(suites); suite_index++) {
    const test_suite *suite = &suites[suite_index];
    printf("\n[%s]\n", suite->name);
    fflush(stdout);

    for (size_t case_index = 0; case_index < suite->count; case_index++) {
      const test_case *test = &suite->cases[case_index];
      total++;
      bool success = test->run();
      if (success) {
        passed++;
        printf("  PASS %s\n", test->name);
      } else {
        failed++;
        printf("  FAIL %s\n", test->name);
      }
      fflush(stdout);
    }
  }

  printf("\nRan %zu tests: %zu passed, %zu failed\n", total, passed, failed);
  return failed == 0 ? 0 : 1;
}
