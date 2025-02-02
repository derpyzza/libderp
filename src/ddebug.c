// #include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "ddebug.h"

#ifdef DLOG_COLORS
#define RESET "\x1b[0m"
static const char* __dlog_colours[DLOG_NUMBER] = {
	"\x1b[35m", // FATAL
	"\x1b[31m", // ERROR
	"\x1b[32m", // WARN
	"\x1b[36m", // INFO
	"\x1b[34m", // TODO
	"\x1b[33m", // DEBUG
};
#endif

void assert_fail (const char* expr, const char* message, const char* file, int line) {
#ifdef DLOG_COLORS
	printf("%s[FAIL]:%s %s at (%s:%i) %s\n", 
		__dlog_colours[1],
		RESET,
		expr,
		file,
		line,
		message);
#else
	printf("[FAIL]: %s at (%s:%i) %s\n", 
		expr,
		file,
		line,
		message);

#endif
}

// I don't think these will be necessary, but i'll keep them here just in case.
// Get string version of the Global log level
// #define G_log_level_pretty log_levels_list[G_log_level]
// Get string version of log level
// #define log_level_pretty(level) log_strings[level]

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

	// time_t current_time;
	// struct tm * m_time;
	// time(&current_time);
	// m_time = localtime(&current_time);

	char* string[32000];
	memset(string, 0, sizeof(string));

	va_list va;
	va_start(va, fmt);	
	vsnprintf((char*)string, 32000, fmt, va);
	va_end(va);
	
	char* out_string[32000];
	memset(out_string, 0, sizeof(out_string));
#ifdef DLOG_COLORS
	sprintf((char*)out_string, "%s%s \x1b[90m%s:%i%s, %s\n",
	// sprintf((char*)out_string, "%d:%d:%d %s%s \x1b[90m%s:%i%s, %s\n",
			// m_time->tm_hour,
			// m_time->tm_min,
			// m_time->tm_sec,
			log_colours[log_level],
			log_strings[log_level],
			file,
			line,
			RESET, 
			(char*)string);
#else
	// sprintf((char*)out_string, "%d:%d:%d [%s] %s:%i, %s\n",
	sprintf((char*)out_string, "[%s] %s:%i, %s\n",
			// m_time->tm_hour,
			// m_time->tm_min,
			// m_time->tm_sec,
			log_strings[log_level],
			file,
			line, 
			(char*)string);
#endif

	printf("%s", (char*)out_string);
}
