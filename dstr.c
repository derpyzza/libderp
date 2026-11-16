#include "derp.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

dstr dstr_new(isize init, d_allocator alloc) {
	dstr str = {0};

	str.len = init;
	str.cptr = dalloc_alloc(alloc, sizeof(char) * str.len);
	if(!str.cptr) {
		dlog_error("Could not allocate memory for string of size %zd", init);
	}

	return str;
}

dstr dstr_from(char * from, d_allocator alloc) {
	isize len = strlen(from);
	dstr str = dstr_new(len, alloc);
	memcpy(str.cptr, from, len);
	return str;
}

dstr dstr_dup(dstr s, d_allocator alloc) {
	dstr out;
	out.len = s.len;
	out.cptr = dalloc_alloc(alloc, out.len);
	memcpy(out.cptr, s.cptr, out.len);
	return out;
}

dstr dstr_append_cstr(dstr dest, const char * src, d_allocator alloc) {
	dassert(dest.cptr != NULL, "dest string must not be NULL");
	dassert(src != NULL, "src string must not be NULL");
	dstr out = {0};

	isize len = strlen(src);

	// allocate new buffer for string
	out.cptr = dalloc_alloc(alloc, sizeof(char) * len + dest.len + 1);
	// FIXME: better error handling ...
	if(!out.cptr) return out;

	for(int i = 0; i < dest.len; i++) out.cptr[i] = dest.cptr[i];
	for(int i = 0; i < len; i++) out.cptr[i+dest.len] = src[i];
	return out;
}

dstr dstr_append (dstr dest, dstr src, d_allocator alloc) {
	dassert(dest.cptr != NULL, "dest string must not be NULL");
	dassert(src.cptr != NULL, "src string must not be NULL");
	dstr out = {0};

	isize len = src.len;

	// allocate new buffer for string
	out.cptr = dalloc_alloc(alloc, sizeof(char) * len + dest.len + 1);
	// FIXME: better error handling ...
	if(!out.cptr) return out;

	for(int i = 0; i < dest.len; i++) out.cptr[i] = dest.cptr[i];
	for(int i = 0; i < len; i++) out.cptr[i+dest.len] = src.cptr[i];
	return out;
}

dstr dstr_prepend_cstr(dstr dest, const char * src, d_allocator alloc) {
	dassert(dest.cptr != NULL, "dest string must not be NULL");
	dassert(src != NULL, "src string must not be NULL");
	dstr out = {0};

	isize len = strlen(src);

	// allocate new buffer for string
	out.cptr = dalloc_alloc(alloc, sizeof(char) * len + dest.len + 1);
	// FIXME: better error handling ...
	if(!out.cptr) return out;

	for(int i = 0; i < len; i++) out.cptr[i] = src[i];
	for(int i = 0; i < dest.len; i++) out.cptr[i+len] = dest.cptr[i];
	return out;
}

dstr dstr_prepend (dstr dest, dstr src, d_allocator alloc) {
	dassert(dest.cptr != NULL, "dest string must not be NULL");
	dassert(src.cptr != NULL, "src string must not be NULL");
	dstr out = {0};

	isize len = src.len;

	// allocate new buffer for string
	out.cptr = dalloc_alloc(alloc, sizeof(char) * len + dest.len + 1);
	// FIXME: better error handling ...
	if(!out.cptr) return out;

	for(int i = 0; i < len; i++) out.cptr[i] = src.cptr[i];
	for(int i = 0; i < dest.len; i++) out.cptr[i+len] = dest.cptr[i];
	return out;
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

dbuf_dstr dstr_split_tokens  (dstr src, const char* tkn, d_allocator alloc) {
	dbuf_dstr sv = dbuf_dstr_make_alloc(8, alloc);

	dstr dup = dstr_dup(src, alloc);
	char *tok = strtok(dup.cptr, tkn);

	while (tok != NULL) {
		dstr s = dstr_from(tok, alloc);
		dbuf_dstr_push(&sv, s);

		tok = strtok(NULL, tkn);
	}

	d_free(dup.cptr);
	return sv;
}
dbuf_dstr dstr_split_lines(dstr src, d_allocator alloc) {
	return dstr_split_tokens(src, "\n", alloc);
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

dstr dstr_replace (dstr str, const char* pat, const char* sub, d_allocator alloc) {
	dstr out = {0};
	todo();
	return out;
}

dstr dstr_replace_all (dstr str, const char* pat, const char* sub, d_allocator alloc) {
	dstr out = {0};
	todo();
	return out;
}

dstr dstr_replace_nth (dstr str, int n, const char* pat, const char* sub, d_allocator alloc) {
	dstr out = {0};
	todo();
	return out;
}

dstr dstr_replace_n   (dstr str, int n, const char* pat, const char* sub, d_allocator alloc) {
	dstr out = {0};
	todo();
	return out;
}
