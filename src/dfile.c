#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/dmem.h"
#include "../include/dfile.h"
#include "../include/ddebug.h"
#include "../include/derror.h"

result read_file(char* path) {
	result res = { 0 };

	res = dalloc(sizeof(File));
	if (res.err) {
		derror(res.err);
		return res;
	}
	File *out = (File *)res.ok;

	out->path = split_path(path);

	FILE *in = fopen(path, "rb");
	if (in == NULL) {
		log_error("could not read file \"%s\"\n", path);
		res.err = D_FILE_READ_ERROR;
		return res;
	}
	fseek(in, 0, SEEK_END); 
	out->data.len = ftell(in); 
	rewind(in); 

	res = dalloc(out->data.len + 1);
	if (res.err) {
		derror(res.err);
		fclose(in);
		return res;
	}

	out->data.cptr = res.ok;

	size bufsread = fread(out->data.cptr, 1, out->data.len, in);
	if ( bufsread < out->data.len ) {
		log_error("could not read enough bytes from file %s\n", path);
		fclose(in);
		return res;
	}

	fclose(in);
	res.ok = out;
	return res;
}

// split the file path on the '.' into 'name' and 'ext' strings
PathInfo split_path(char* raw_path) {
	size len = strlen(raw_path);
	char* name = (char*) dalloc(sizeof(char) * len).ok;
	int id = 0;
	char* c = raw_path;
	while(*c != '.') {
		name[id] = *c;
		id++;
		c++;
	}
	name[id] = '\0';

	char* ext = (char*)dalloc((len - id) * sizeof(char) + 1).ok;

	if(ext != NULL) strcpy(ext, c);
	else log_error("could not malloc file ext\n");
	return (PathInfo){ 
		.full_path = raw_path,
		.path = name,
		.ext = ext
	};
}

result read_file_to_lines(char *path) {
	result res = read_file(path);
	if (res.err) { 
		return res;
	} else { 
		File *f = res.ok;
		f->is_lines = true;
		f->lines = dstr_split_lines(&f->data);
		return res;
	}
}

