#pragma once

#include <stdlib.h>

#include "dtypes.h"
#include "dvec.h"

// Dynamically sized string
typedef struct dstr {
	int len; // not size, because the printf string format expects an int as the length
	int cap;
	char* cptr;
} dstr;

// create new dynamic string, with an initial size of `init`
dstr *dstr_new(size init);
// create new dynamic string, using the string `s` as the base string
dstr *dstr_from(const char* s);
// append d-string `s` to d-string `o`
void dstr_append_dstr(dstr* o, const dstr* s);
// append c string ( char * ) `s` to `o`
void dstr_append_cstr(dstr* o, const char* s);

dstr* dstr_dup (const dstr* s);


// ASCII only for now
bool char_is_digit(char c);
bool char_is_alpha(char c);
bool char_is_alphanum(char c);
u8 char_to_digit(char c);

result dstr_parse_int(dstr src);
result dstr_parse_float(dstr src);

typedef struct {
	dstr* data;
	size len;
	size cap;
} vecstr;

DECL_VEC_TYPE(dstr*, str)

dvec_str* dstr_split_lines  (dstr *src);
dvec_str* dstr_split_tokens (dstr *src, const char* tkn);
