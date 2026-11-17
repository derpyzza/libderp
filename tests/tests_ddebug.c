#include "tests.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct output_capture {
  FILE *output;
  int saved_stdout;
} output_capture;

static bool capture_begin(output_capture *capture) {
  capture->output = NULL;
  capture->saved_stdout = -1;
  fflush(stdout);
  capture->output = tmpfile();
  if (capture->output == NULL) {
    return false;
  }
  capture->saved_stdout = dup(STDOUT_FILENO);
  if (capture->saved_stdout < 0 ||
      dup2(fileno(capture->output), STDOUT_FILENO) < 0) {
    if (capture->saved_stdout >= 0) {
      close(capture->saved_stdout);
    }
    fclose(capture->output);
    capture->output = NULL;
    return false;
  }
  return true;
}

static bool capture_end(output_capture *capture, char *buffer,
                        size_t buffer_size) {
  bool success = true;
  fflush(stdout);
  if (dup2(capture->saved_stdout, STDOUT_FILENO) < 0) {
    success = false;
  }
  close(capture->saved_stdout);
  rewind(capture->output);
  size_t count = fread(buffer, 1, buffer_size - 1, capture->output);
  if (ferror(capture->output)) {
    success = false;
  }
  buffer[count] = '\0';
  fclose(capture->output);
  capture->output = NULL;
  capture->saved_stdout = -1;
  return success;
}

static bool test_logging_formats_messages_and_arguments(void) {
  output_capture capture;
  char output[1024];
  TEST_ASSERT(capture_begin(&capture));
  dlog_init(DLOG_INFO);
  dlog_info("value=%d", 42);
  dlog_debug("hidden debug message");
  TEST_ASSERT(capture_end(&capture, output, sizeof(output)));

  TEST_ASSERT(strstr(output, "[INFO]") != NULL);
  TEST_ASSERT(strstr(output, "value=42") != NULL);
  TEST_ASSERT(strstr(output, "tests_ddebug.c") != NULL);
  TEST_ASSERT(strstr(output, "hidden debug message") == NULL);
  return true;
}

static bool test_logging_filters_levels(void) {
  output_capture capture;
  char output[2048];
  TEST_ASSERT(capture_begin(&capture));
  dlog_init(DLOG_ERROR);
  dlog_warn("hidden warning");
  dlog_error("visible error");
  dlog_init(DLOG_TRACE);
  dlog_trace("visible trace");
  TEST_ASSERT(capture_end(&capture, output, sizeof(output)));

  TEST_ASSERT(strstr(output, "hidden warning") == NULL);
  TEST_ASSERT(strstr(output, "[ERRO]") != NULL);
  TEST_ASSERT(strstr(output, "visible error") != NULL);
  TEST_ASSERT(strstr(output, "[TRAC]") != NULL);
  TEST_ASSERT(strstr(output, "visible trace") != NULL);
  return true;
}

static bool test_successful_assertion_continues(void) {
  int value = 7;
  dassert(value == 7, "value should be seven");
  TEST_ASSERT(value == 7);
  return true;
}

const test_case tests_ddebug[] = {
    TEST_CASE(test_logging_formats_messages_and_arguments),
    TEST_CASE(test_logging_filters_levels),
    TEST_CASE(test_successful_assertion_continues),
};
const size_t tests_ddebug_count = TEST_ARRAY_LEN(tests_ddebug);
