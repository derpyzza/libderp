#include "derp.h"

u64 dfile_get_size(char* path) {
	FILE *in = fopen(path, "rb");
	dassert(in != NULL, "Could not open file [%s], check if it exists?\n", path);

	fseek(in, 0, SEEK_END); 
	u64 len = ftell(in); 
	rewind(in); 
	fclose(in);
	return len;
}


dstr dfile_read(char* path) {
  dassert(path != NULL, "Filepath is NULL");

	FILE *in = fopen(path, "rb");
	dassert(in != NULL, "Could not open file [%s], check if it exists?\n", path);

	dstr out = dstr("");

	fseek(in, 0, SEEK_END); 
	out.len = ftell(in); 
	rewind(in); 

	out.cptr = d_alloc(out.len + 1);

	isize bufsread = fread(out.cptr, 1, out.len, in);
	if ( bufsread < out.len ) {
		dlog_error("could not read enough bytes from file %s, bytes read: %zd, bytes needed: %zd\n", path, bufsread, out.len);
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
		dlog_error("could not read enough bytes from file %s, bytes read: %zd, bytes needed: %zd\n", path, bufsread, out.len);
		fclose(in);
		return out;
	}
	fclose(in);
	return out;
}

dbuf_dstr dfile_read_lines(char *path) {
	dstr s = dfile_read(path);
	dbuf_dstr out = dstr_split_lines(s);
	d_free(s.cptr);
	return out;
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
