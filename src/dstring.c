#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/dvec.h"
#include "../include/dstring.h"
#include "../include/ddebug.h"
#include "../include/dfile.h"

dstr *dstr_new(size init) {
	dstr *str = (dstr*)malloc(sizeof(dstr));

	str->len = init;
	str->cptr = malloc(sizeof(char) * str->len);

	if(str) return str;
	else return NULL;
}

dstr *dstr_from(const char *s) {
	dstr *str = (dstr*)malloc(sizeof(struct dstr));
	str->len = str->cap = strlen(s);
	str->cptr = (char*)malloc(sizeof(char) * str->len);
	if (memcpy(str->cptr, s, str->len)) return str;
	else PANIC("Error: Could not copy memory for new string");
}

void dstr_grow(dstr* s, size len) {
	if (s->len + len < s->cap ) return;

	while ( s->len + len > s->cap )
		s->cap *= 2;
	s->cptr = (char*)realloc(s->cptr, sizeof(char) * s->cap);
}

u8 char_to_digit(char c) {
	return c - '0';
}

bool char_is_digit(char c) {
	return (c >= '0' && c <= '9');
}

// result str_parse_float(dstr src) {
// 	f64 num = 0;
// 	char * st = src.cptr;
// 	for (char * c = src.cptr; c-st != src.len; c++) {
// 		while(char_is_digit(*c)) {
// 			if (num == 0) num = char_to_digit(*c);
// 			else num = num * 10 + char_to_digit(*c);
// 		}
// 		if (*c == '.' && char_is_digit(*(c+1))) {
// 			while(char_is_digit(*c)) {
				
// 			}
// 		}
// 	}
// }

void dstr_append_cstr(dstr* o, const char* s) {
	size len = strlen(s);
	dstr_grow(o, len);

	memcpy(o->cptr + o->len, s, 1 + (sizeof(char) * len));
	o->len += len;
}

dstr* dstr_dup (const dstr* s) {
	return dstr_from(strdup(s->cptr));
}

dvec_str *dstr_split_tokens (dstr *src, const char *tkn) {
	dvec_str *sv = new_dvec_str(1);
	dstr* dup = dstr_dup(src);
	char * tok = strtok(dup->cptr, tkn);

	while (tok != NULL) {
		dstr* s = dstr_from(tok);
		if (s) dvec_push_str(sv, s);
		else PANIC("Error: Could not create new string");

		tok = strtok(NULL, tkn);
	}

	return sv;
}

dvec_str *dstr_split_lines(dstr * f) {
	return dstr_split_tokens(f, "\n");
}

