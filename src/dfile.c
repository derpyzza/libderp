#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/dfile.h"
#include "../include/ddebug.h"

struct File *read_file(char* path) {
	File *out = (File*)malloc(sizeof(File));
	out->path = split_path(path);
	out->is_valid = false;

	FILE *in = fopen(path, "rb");
	if (in == NULL) {
		printf("Error, could not read file %s\n", path);
		return out;
	}
	fseek(in, 0, SEEK_END); 
	out->data.len = ftell(in); 
	rewind(in); 

	out->data.cptr = (char*)malloc(out->data.len + 1);
	if (out->data.cptr == NULL) {
		log_error("could not allocate enough memory for file %s\n", path);
		fclose(in);
		return out;
	}

	size bufsread = fread(out->data.cptr, 1, out->data.len, in);
	if ( bufsread < out->data.len ) {
		log_error("could not read enough bytes from file %s\n", path);
		fclose(in);
		return out;
	}

	fclose(in);
	out->is_valid = true;
	return out;
}

// split the file path on the '.' into 'name' and 'ext' strings
FilePath split_path(char* raw_path) {
	size len = strlen(raw_path);
	char* name = malloc(sizeof(char) * len);
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

	else log_error("could not malloc ext file\n");
	return (FilePath){ 
		.full_path = raw_path,
		.path = name,
		.ext = ext
	};
}

