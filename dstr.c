#include "derp.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dstr dstr_new(isize init) {
	dstr str = {0};

	str.len = init;
	str.cptr = d_alloc(sizeof(char) * str.len);
	if(!str.cptr) {
		dlog_error("Could not allocate memory for string of size %zd", init);
	}

	return str;
}

dstr dstr_from(char * from) {
	isize len = strlen(from) + 1;
	dstr str = dstr_new(len);
	memcpy(str.cptr, from, len);
	return str;
}

int dstr_concat_cstr(dstr *dest, char * src) {
	dassert(dest != NULL, "dest string must not be NULL");
	dassert( src != NULL, "src string must not be NULL");

	isize len = strlen(src);

	// allocate new buffer for string
	char * tmp = d_alloc(sizeof(char) * len + dest->len + 1);
	// FIXME: better error handling ...
	if(!tmp) return -1;

	for(int i = 0; i < dest->len; i++) tmp[i] = dest->cptr[i];
	for(int i = 0; i < len; i++) tmp[i+dest->len] = src[i];
	dest->cptr = tmp;
	dest->len += len;
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
	if(to->len < from->len) {
		to->cptr = realloc(to->cptr, from->len);
	} 

	memcpy(to->cptr, from->cptr, from->len);
}

dstr dstr_dup (dstr s) {
	dstr out;
	out.len = s.len;
	out.cptr = d_alloc(out.len);
	memcpy(out.cptr, s.cptr, out.len);
	return out;
}

dbuf_dstr dstr_split_tokens (dstr src, const char *tkn) {
	dbuf_dstr sv = dbuf_dstr_make(8);

	dstr dup = dstr_dup(src);
	char *tok = strtok(dup.cptr, tkn);

	while (tok != NULL) {
		dstr s = dstr_from(tok);
		dbuf_dstr_push(&sv, s);

		tok = strtok(NULL, tkn);
	}

	d_free(dup.cptr);
	return sv;
}

dbuf_dstr dstr_split_lines(dstr f) {
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


void dstr_to_lower( dstr s ) {
	for(int i = 0; i < s.len; i++) {
		s.cptr[i] = (char)tolower(s.cptr[i]);
	}
}

void dstr_to_upper( dstr s ) {
	for(int i = 0; i < s.len; i++) {
		s.cptr[i] = (char)toupper(s.cptr[i]);
	}
}

void dstr_replace (dstr * str, const char* pat, const char* sub) {
	todo();
	char * _sub = strstr(str->cptr, pat);

	if(_sub == NULL && strcmp(pat, sub) == 0) return;

	isize len = strlen(pat);
	dstr_grow(str, 512);
	sprintf(_sub, "%s%s", sub, _sub + len);
}

void dstr_replace_all (dstr *str, const char* pat, const char* sub) {
	todo();
}

void dstr_replace_nth (dstr *str, int n, const char* pat, const char* sub) {
	todo();
}

void dstr_replace_n   (dstr *str, int n, const char* pat, const char* sub) {
	todo();
}
