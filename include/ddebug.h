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
	LOG_FATAL = 0,
	LOG_ERROR = 1,
	LOG_WARN = 2,
	LOG_INFO = 3,
	LOG_TODO = 4,
	LOG_DEBUG = 5,
	LOG_NUMBER
} LogLevel;

// for now the log levels are just hardcoded into the code.
// later i plan on making a nicer logging system but this is good enough for now
//
// warning: "good enough for now" often translates to "this is a going to be a permanent feature"
#ifdef DEBUG
#define LOG_LEVEL LOG_DEBUG
#else
#define LOG_LEVEL LOG_ERROR
#endif

#define log_fatal(...)	log_log(LOG_FATAL,__FILE__, __LINE__,__VA_ARGS__)
#define log_error(...)	log_log(LOG_ERROR,__FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)	log_log(LOG_WARN,__FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)	log_log(LOG_INFO,__FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...)	log_log(LOG_DEBUG,__FILE__, __LINE__, __VA_ARGS__)
#define log_todo(...)	log_log(LOG_TODO,__FILE__, __LINE__, __VA_ARGS__)

void log_log(LogLevel log_level, const char* file, int line, const char* fmt, ...);
// void log_init(LogLevel level);

// convenience macro for checking null pointer dereferences
#define CHKPTR(ptr) if (ptr == NULL)	\
log_fatal("Null pointer dereference exception"),\
exit(-1);

#define PANIC(msg) log_fatal(msg), exit(-1);
#define TODO(ret, msg) { log_debug("TODO: %s", msg); return ret; }
