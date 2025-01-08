/** @file handy debugging + logging functions */
#pragma once

#if DEBUG == 1
  #if _MSC_VER
    #include <intrin.h>
    #define BREAK() __debugbreak()
  #else
    #define BREAK() __builtin_trap()
  #endif

#define ASSERT(expr, msg)																			\
{																															\
	if (expr) {}																								\
	else {																											\
		assertion_function(#expr, msg, __FILE__, __LINE__); 			\
	}																														\
}

void assert_fail (const char* expr, const char* message, const char* file, int line);

#else 
  #define ASSERT(expr, msg)
#endif


typedef enum {
	DLOG_FATAL = 0,
	DLOG_ERROR = 1,
	DLOG_WARN = 2,
	DLOG_INFO = 3,
	DLOG_TODO = 4,
	DLOG_DEBUG = 5,
	DLOG_NUMBER
} DLogLevel;

// for now the log levels are just hardcoded into the code.
// later i plan on making a nicer logging system but this is good enough for now
//
// warning: "good enough for now" often translates to "this is a going to be a permanent feature"
#ifdef DEBUG
#define DLOG_LEVEL DLOG_DEBUG
#else
#define DLOG_LEVEL DLOG_ERROR
#endif

#define dlog_fatal(...) dlog_log(DLOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)
#define dlog_error(...) dlog_log(DLOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define dlog_warn(...)	dlog_log(DLOG_WARN , __FILE__, __LINE__, __VA_ARGS__)
#define dlog_info(...)	dlog_log(DLOG_INFO , __FILE__, __LINE__, __VA_ARGS__)
#define dlog_debug(...) dlog_log(DLOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define dlog_todo(...)  dlog_log(DLOG_TODO , __FILE__, __LINE__, __VA_ARGS__)

void dlog_log(DLogLevel log_level, const char* file, int line, const char* fmt, ...);
// void log_init(LogLevel level);

// convenience macro for checking null pointer dereferences
#define CHKPTR(ptr) if (ptr == NULL)	\
log_fatal("Null pointer dereference exception"),\
exit(-1);

#define PANIC(msg) dlog_fatal(msg), exit(-1);
#define TODO(ret, msg) { dlog_debug("TODO: %s", msg); return ret; }
