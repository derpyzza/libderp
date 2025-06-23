// #include <time.h>
#include "derp.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifdef DLOG_COLORS
#define RESET "\x1b[0m"
static const char* __dlog_colours[DLOG_NUMBER] = {
	"\x1b[35m", // FATAL
	"\x1b[31m", // ERROR
	"\x1b[32m", // WARN
	"\x1b[36m", // INFO
	"\x1b[33m", // DEBUG
	"\x1b[34m", // TODO
};
#endif

void assert_fail (const char* expr, const char* file, int line) {
#ifdef DLOG_COLORS
	printf("%s[ASSERT_FAIL]:%s (%s:%i):\n\texpr: %s\n", 
		__dlog_colours[1],
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

void dlog_log( DLogLevel log_level, const char* file, int line, const char* fmt, ...) {
	// string version of log levels
	const char* log_strings[DLOG_NUMBER] = {
		"FTAL",
		"ERRO",
		"WARN",
		"INFO",
		"TODO",
		"DEBG"
	};

	if (log_level > DLOG_LEVEL) {
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
			__dlog_colours[log_level],
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

	printf("%s", (char*)out_string);
}
