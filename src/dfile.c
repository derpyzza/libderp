#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/dmem.h"
#include "../include/dfile.h"
#include "../include/ddebug.h"
#include "../include/derror.h"

File* read_file(char* path) {
	File* out = malloc(sizeof(File));
	if (!out) {
		return out;
	}
	out->is_valid = false;
	out->path = split_path(path);

	FILE *in = fopen(path, "rb");
	if (in == NULL) {
		dlog_error("could not read file \"%s\"\n", path);
		return out;
	}
	fseek(in, 0, SEEK_END); 
	out->data.len = ftell(in); 
	rewind(in); 

	result d = dalloc(out->data.len + 1);
	if (d.err) {
		derror(d.err);
		fclose(in);
		return out;
	}

	out->data.cptr = d.ok;

	size bufsread = fread(out->data.cptr, 1, out->data.len, in);
	if ( bufsread < out->data.len ) {
		dlog_error("could not read enough bytes from file %s\n", path);
		fclose(in);
		return out;
	}

	fclose(in);
	out->is_valid = true;
	return out;
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
	else dlog_error("could not malloc file ext\n");
	return (PathInfo){ 
		.full_path = raw_path,
		.path = name,
		.ext = ext
	};
}

File* read_file_to_lines(char *path) {
	File* f = read_file(path);
	if (!f->is_valid) { 
		return f;
	} else { 
		f->is_lines = true;
		f->lines = dstr_split_lines(&f->data);
		return f;
	}
}

