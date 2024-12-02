#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/dstring.h"
#include "../include/ddebug.h"

String *str_new(size init) {
	String *str = (String*)malloc(sizeof(String));

	str->len = init;
	str->cptr = malloc(sizeof(char) * str->len);

	if(str) return str;
	else return NULL;
}

String *str_from(const char *s) {
	String *str = (String*)malloc(sizeof(struct String));
	str->len = str->cap = strlen(s);
	str->cptr = (char*)malloc(sizeof(char) * str->len);
	if (memcpy(str->cptr, s, str->len)) return str;
	else PANIC("Error: Could not copy memory for new string");
}

void str_grow(String* s, size len) {
	if (s->len + len < s->cap ) return;

	while ( s->len + len > s->cap )
		s->cap *= 2;
	s->cptr = (char*)realloc(s->cptr, sizeof(char) * s->cap);
}

void str_append(String* o, char* s) {
	size len = strlen(s);
	str_grow(o, len);

	memcpy(o->cptr + o->len, s, len);
	o->len += len;
}
