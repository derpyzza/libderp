/** @file file manipulation stuff */
#pragma once

#include "dtypes.h"
#include "dstring.h"

typedef struct FilePath {
	char* full_path; 	// the full path, including the extension
	char* path; 			// the path, excluding the extension
	char* name;			 	// the file name
	char* ext; 				// the file extension
} FilePath;

typedef struct File {
	bool is_valid;
	FilePath path;
	String data;
} File;


FilePath split_path(char* path);
File *read_file(char* path);
