// #include <time.h>
#include "derp.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct dlogger {
  DLogLevel level;
  FILE * f_out;
} def_logger;

#ifdef DLOG_COLORS
#define RESET "\x1b[0m"
static const char* dlog_colours[DLOG_NUMBER] = {
	#define DLOG(e, s, c) c,
	DLOG_LEVEL_DATA
	#undef DLOG
};
#endif

void dlog_func(DLogLevel log_level, const char* file, int line, const char* fmt, ...);

// string version of log levels
const char* log_strings[DLOG_NUMBER] = {
	#define DLOG(e, s, c) s,
	DLOG_LEVEL_DATA
	#undef DLOG
};

void assert_fail (const char* expr, const char* file, int line) {
#ifdef DLOG_COLORS
	printf("%s[ASSERT_FAIL]:%s (%s:%i):\n\texpr: %s\n", 
		dlog_colours[1],
		RESET,
		file,
		line,
		expr);
	__builtin_trap();
#else
	printf("[FAIL]: %s at (%s:%i)\n", 
		expr,
		file,
		line );

#endif
}

void dlog_init(DLogLevel lvl) {
	def_logger.level = lvl;
	def_logger.f_out = stdout;
}

void dlog_func ( DLogLevel log_level, const char* file, int line, const char* fmt, ...) {
	if (log_level > def_logger.level) {
		return;
	}

	char* string[32000];
	memset(string, 0, sizeof(string));

	va_list va;
	va_start(va, fmt);	
	vsnprintf((char*)string, 32000, fmt, va);
	va_end(va);
	
	char* out_string[32000];
	memset(out_string, 0, sizeof(out_string));
#ifdef DLOG_COLORS
	sprintf((char*)out_string, "%s[%s] \x1b[90m%s:%i%s, %s\n",
			dlog_colours[log_level],
			log_strings[log_level],
			file,
			line,
			RESET, 
			(char*)string);
#else
	sprintf((char*)out_string, "[%s] %s:%i, %s\n",
			log_strings[log_level],
			file,
			line, 
			(char*)string);
#endif

	fprintf(def_logger.f_out, "%s", (char*)out_string);
}
