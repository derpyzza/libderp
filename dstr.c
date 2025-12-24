#include "derp.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

dstr dstr_new(isize init) {
	dstr str = {0};

	str.len = init;
	str.cptr = d_alloc(sizeof(char) * str.len);
	if(!str.cptr) {
		dlog_error("Could not allocate memory for string of size %lli", init);
	}

	return str;
}

dstr dstr_from(char * from) {
	isize len = strlen(from) + 1;
	dstr str = dstr_new(len);
	memcpy(str.cptr, from, len);
	return str;
}

int dstr_append_cstr(dstr *s, const char* c) {
  if(!c) return -1;
	isize len = strlen(c);
	// allocate new buffer for string
	char * tmp = d_alloc(sizeof(char) * len + s->len + 1);
	if(!tmp) return -1;
	for(int i = 0; i < s->len; i++) tmp[i] = s->cptr[i];
	for(int i = 0; i < len; i++) tmp[i+s->len] = c[i];
	s->cptr = tmp;
	s->len += len;
	return 0;
}

bool dstr_eq(dstr a, dstr b) {
  if(a.len != b.len) {
    return 0;
  }
  for(int i = 0; i < b.len; i++) {
    if(a.cptr[i] != b.cptr[i]) {
      return 0;
    }
  }
  return 1;
}

void dstr_copy(dstr* to, dstr* from) {
	dlog_debug("to: %s, from: %s", to->cptr, from->cptr);
  for(int i = 0; i < from->len; i++) {
    to->cptr[i] = from->cptr[i];
  }
}

dstr dstr_dup (dstr s) {
  const char * str = strdup(s.cptr);
	return dstr((char *)str);
}

dbuf dstr_split_tokens (dstr src, const char *tkn) {
	dbuf sv  = dbuf_new(dstr*, 1);
		if(!sv.data) {
			dlog_error("Could not split tokens");
			return sv;
		}
	dstr dup = dstr_dup(src);
	char *tok = strtok(dup.cptr, tkn);

	while (tok != NULL) {
		dstr s = dstr(tok);
		dbuf_push(sv, &s);
		// else PANIC("Error: Could not create new string");

		tok = strtok(NULL, tkn);
	}

	return sv;
}

dbuf dstr_split_lines(dstr f) {
	return dstr_split_tokens(f, "\n");
}

void dstr_grow(dstr* s, isize len) {
	s->len += len;
	s->cptr = (char*)realloc(0, sizeof(char) * s->len);
}

u8 d_char_to_digit(char c) {
	return c - '0';
}

bool d_char_is_digit(char c) {
	return (c >= '0' && c <= '9');
}

bool d_char_is_alpha(char c) {
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

bool d_char_is_alphanum(char c) {
	return d_char_is_alpha(c) || d_char_is_digit(c);
}
