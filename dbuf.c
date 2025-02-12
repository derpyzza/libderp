#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dbuf.h"
#include "ddebug.h"

dbuf *dbuf_make_new(size init, size elem_size) {
	dbuf *v = malloc(sizeof(dbuf));
	if(v) {
		v->cap = init;
		v->current = 0;
		v->data = calloc(v->cap, sizeof(elem_size));
		return v;
	} else {
		dlog_error("Not enough memory to create new buf");
		return NULL;
	}
}

int dbuf_grow(dbuf* v, size size) {
	v->cap += size;
	void* data = realloc(v->data, sizeof(v->elem_size) * v->cap);
	if(data) {
		v->data = data;
		return 0;
	}
	return -1;
}

dbuf *dbuf_init_new(void* data, size len, size elem_size) {
	dbuf *v = dbuf_make_new(len, sizeof(elem_size));
	memcpy(data, v->data, elem_size * len);

	if (v) {
		return v;	
	} else {
		dlog_error("Couldn't create new vec, copy fail");
		return NULL;
	}
}

int dbuf_push(dbuf *v, void* item) {
	if (v->current + 1 > v->cap) {
		if(dbuf_grow(v, v->cap * 2) < 0) return -1;
	}

	v->data[v->current] = item;
	v->current++;
	return 0;
}

void* dbuf_pop(dbuf *v) {
	if (v->current == 0) {
		return 0;
	}

	return v->data[--v->current];
}

dstr *dstr_new(size init) {
	dstr *str = (dstr*)malloc(sizeof(dstr));
	if(!str) return NULL;

	str->len = init;
	str->cptr = malloc(sizeof(char) * str->len);
	if(!str->cptr) return NULL;

	return str;
}

dstr *dstr_from(const char *s) {
	dstr *str = (dstr*)malloc(sizeof(struct dstr));
	if(!str) return NULL;

	str->len = str->cap = strlen(s);
	str->cptr = (char*)malloc(sizeof(char) * str->len);
	if(!str->cptr) return NULL;

	if (memcpy(str->cptr, s, str->len)) {
		return str;
	} else {
		dlog_error("could not copy memory for new string");
		return NULL;
	}
}

void dstr_grow(dstr* s, size len) {
	if (s->len + len < s->cap ) return;
	s->cap += len;
	s->cptr = (char*)realloc(s->cptr, sizeof(char) * s->cap);
}

u8 char_to_digit(char c) {
	return c - '0';
}

bool char_is_digit(char c) {
	return (c >= '0' && c <= '9');
}

// result dstr_parse_float(dstr src) {
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

dbuf *dstr_split_tokens (dstr *src, const char *tkn) {
	dbuf *sv  = dbuf_new(dstr*, 1);
		if(!sv) return NULL;
	dstr *dup = dstr_dup(src);
	char *tok = strtok(dup->cptr, tkn);

	while (tok != NULL) {
		dstr* s = dstr_from(tok);
		if (s) dbuf_push(sv, s);
		else PANIC("Error: Could not create new string");

		tok = strtok(NULL, tkn);
	}

	return sv;
}

dbuf *dstr_split_lines(dstr * f) {
	return dstr_split_tokens(f, "\n");
}

u64 dfile_get_size(char* path) {
	FILE *in = fopen(path, "rb");
	if (!in) {
		dlog_error("could not open file [%s]\n", path);
		return 0;
	}
	fseek(in, 0, SEEK_END); 
	u64 len = ftell(in); 
	rewind(in); 
	fclose(in);
	return len;
}

dstr* dfile_read(char* path) {
	FILE *in = fopen(path, "rb");
	if (!in) {
		dlog_error("could not open file [%s], check if it exists?\n", path);
		return 0;
	}
	dstr* out = malloc(sizeof(dstr));
	fseek(in, 0, SEEK_END); 
	out->len = ftell(in); 
	rewind(in); 

	out->cptr = malloc(out->len + 1);
	if (out == NULL) {
		fclose(in);
		return 0;
	}

	size bufsread = fread(out->cptr, 1, out->len, in);
	if ( bufsread < out->len ) {
		dlog_error("could not read enough bytes from file %s, bytes read: %i, bytes needed: %i\n", path, bufsread, out->len);
		fclose(in);
		return 0;
	}
	fclose(in);
	return out;
}

dstr* dfile_read_bytes(char* path, u64 bytes) {
	FILE *in = fopen(path, "rb");
	if (!in) {
		dlog_error("could not open file [%s], check if it exists?\n", path);
		return 0;
	}
	dstr* out = dstr_new(bytes);

	out->cptr = malloc(out->len + 1);
	if (out == NULL) {
		fclose(in);
		return 0;
	}

	size bufsread = fread(out->cptr, 1, out->len, in);
	if ( bufsread < out->len ) {
		dlog_error("could not read enough bytes from file %s, bytes read: %i, bytes needed: %i\n", path, bufsread, out->len);
		fclose(in);
		return 0;
	}
	fclose(in);
	return out;
}

dbuf *dfile_read_lines(char *path) {
	dstr *s = dfile_read(path);
	if (!s) { 
		return 0;
	}
	return dstr_split_lines(s);
}

// split the file path on the '.' into 'name' and 'ext' strings
dfilepath split_path(char* raw_path) {
	size len = strlen(raw_path);
	char* name = (char*) malloc(sizeof(char) * len);
	int id = 0;
	char* c = raw_path;
	while(*c != '.') {
		name[id] = *c;
		id++;
		c++;
	}
	name[id] = '\0';

	char* ext = (char*)malloc((len - id) * sizeof(char) + 1);

	if(ext != NULL) strcpy(ext, c);
	else dlog_error("could not malloc file ext\n");
	return (dfilepath){ 
		.full_path = raw_path,
		.path = name,
		.ext = ext
	};
}
