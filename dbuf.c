#include "derp.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

dbuf dbuf_make_new(isize init, isize elem_size) {
	dbuf v = {0};
	v.cap = init;
	v.current = 0;
	v.data = calloc(v.cap, sizeof(elem_size));
	return v;

	dlog_error("Not enough memory to create new buf");
}

dbuf dbuf_init_new(void* data, isize len, isize elem_size) {
	dbuf v = dbuf_make_new(len, sizeof(elem_size));
	memcpy(data, v.data, elem_size * len);

	if (!v.data) {
		dlog_error("Couldn't create new vec, copy fail");
	}
	return v;	
}

void *dbuf_getc(dbuf* v) {
	return v->data[v->current];
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

int dbuf_grow(dbuf* v, isize size) {
	v->cap += size;
	void* data = realloc(v->data, sizeof(v->elem_size) * v->cap);
	if(data) {
		v->data = data;
		return 0;
	}
	return -1;
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

dstr dfile_read(char* path) {
	FILE *in = fopen(path, "rb");
	dstr out = dstr("");
	if (!in) {
		dlog_error("could not open file [%s], check if it exists?\n", path);
		return out;
	}
	fseek(in, 0, SEEK_END); 
	out.len = ftell(in); 
	rewind(in); 

	out.cptr = d_alloc(out.len + 1);
	if (out.cptr == NULL) {
		fclose(in);
		return out;
	}

	isize bufsread = fread(out.cptr, 1, out.len, in);
	if ( bufsread < out.len ) {
		dlog_error("could not read enough bytes from file %s, bytes read: %i, bytes needed: %i\n", path, bufsread, out.len);
		fclose(in);
		return out;
	}
	fclose(in);
	return out;
}

dstr dfile_read_bytes(char* path, u64 bytes) {
	FILE *in = fopen(path, "rb");
	dstr out = dstr_new(bytes + 1);
	if (!in) {
		dlog_error("could not open file [%s], check if it exists?\n", path);
		return out;
	}

	if (out.cptr == NULL) {
		fclose(in);
		return out;
	}

	isize bufsread = fread(out.cptr, 1, out.len, in);
	if ( bufsread < out.len ) {
		dlog_error("could not read enough bytes from file %s, bytes read: %i, bytes needed: %i\n", path, bufsread, out.len);
		fclose(in);
		return out;
	}
	fclose(in);
	return out;
}

dbuf dfile_read_lines(char *path) {
	dstr s = dfile_read(path);
	return dstr_split_lines(s);
}

// split the file path on the '.' into 'name' and 'ext' strings
dfilepath split_path(char* raw_path) {
	isize len = strlen(raw_path);
	char* name = (char*) d_alloc(sizeof(char) * len);
	int id = 0;
	char* c = raw_path;
	while(*c != '.') {
		name[id] = *c;
		id++;
		c++;
	}
	name[id] = '\0';

	char* ext = (char*)d_alloc((len - id) * sizeof(char) + 1);

	if(ext != NULL) strcpy(ext, c);
	else dlog_error("could not malloc file ext\n");
	return (dfilepath){ 
		.full_path = dstr(raw_path),
		.path = dstr(name),
		.ext = dstr(ext)
	};
}
