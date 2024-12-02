#pragma once

#include "dtypes.h"

typedef struct String {
	int len; // not size, because the string format expects an int as the length
	int cap;
	char* cptr;
} String;

String *str_new(size init);
String *str_from(const char* s);
void str_append(String* o, char* s);

// here only temporarily, should be moved into it's own module
// linked list of strings
typedef struct Rope {
	String string;
	String *next; // pointer to next string in list
} Rope ;
