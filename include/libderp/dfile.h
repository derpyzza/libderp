/** @file file manipulation stuff */
#pragma once

#include "dtypes.h"
#include "dstring.h"

typedef struct PathInfo {
	char* full_path; 	// the full path, including the extension
	char* path; 			// the path, excluding the extension
	char* name;			 	// the file name
	char* ext; 				// the file extension
} PathInfo;

typedef struct File {
	bool is_valid;
	bool is_lines;
	PathInfo path;
	union {
		dstr data;
		dvec_str *lines;
	};
} File;

File* read_file(char* path);
File* read_file_to_lines(char* path);
void write_file(dstr *data);
PathInfo split_path(char* path);
