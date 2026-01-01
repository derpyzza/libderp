#include "derp.h"

#include <stdlib.h>

void _dbuf_make (isize size, void** data, isize * len, isize * cur, isize elem_size) {
	dassert(size > 0, "[dbuf] Cannot make a dbuf of size %zd", size);

	*cur = 0;
	*len = size;
	*data = d_calloc(*len, elem_size);
}


void _dbuf_init ( void ** from, isize size, void ** data, isize * len, isize * cur, isize elem_size) {
	dassert(size > 0, "[dbuf] Cannot make a dbuf of size %zd", size);
	dassert(from != NULL, "[dbuf] Cannot copy NULL data");

	_dbuf_make(size, data, len, cur, elem_size);	
	// printf("===DATA===[0] = %c\n", *(int*)&data);
	memcpy(*data, *from, size * elem_size);
	*cur += size;
}


void _dbuf_push( void *item, void **data, isize *len, isize *cur, isize elem_size ) {
	dassert(*cur <= *len
	  , "[dbuf]: current item must not exceed dbuf length"
	);
	dassert(!(*cur > 0 && !*data)
	  , "[dbuf]: possible memory corruption, data mustn't be null when cur is greater than 0"
	);
	dassert(elem_size > 0, "[dbuf] elem_size must not be zero or negative");

	if (*cur >= *len) {
		int size;
		if (*len == 0) size = 32;
		else size = *len * 2;
		*len = size;

		*data = d_realloc(*data, size * elem_size);
	}

	char *addr = (char*)(*data) + (*cur * elem_size);
	memcpy(addr, item, elem_size);
	(*cur)++;
}


void* _dbuf_pop  (void *data, isize *cur, isize elem_size) {
	dassert(*cur > 0, "[dbuf] Cannot pop off a negative index");
	dassert(data != NULL, "[dbuf] Data is NULL, possible memory corruption");

	(*cur)--;
	return (char*)data + (*cur * elem_size);
}


void _dbuf_insert ( void *item, isize index, void **data, isize *len, isize *cur, isize elem_size ) {
	dassert(index >= 0 && index <= *cur, "[dbuf] Cannot access array out of bounds, index: %zd, dbuf current length: %zd.", index, *cur);
	dassert(data != NULL, "[dbuf] Data is NULL, possible memory corruption.");
	dassert(item != NULL, "[dbuf] Cannot insert NULL item into dbuf.");

	if (*cur >= *len) {
		int size;
		if (*len == 0) size = 32;
		else size = *len * 2;
		*len = size;

		*data = d_realloc(*data, size * elem_size);
	}

	if(index < *cur) {
		memmove(
		  (char*)(*data) + ((index + 1) * elem_size)
		  , (char*)(*data) + (index * elem_size)
		  , (*cur - index) * elem_size
		);
	}

	char *addr = (char*)(*data) + (index * elem_size);
	memcpy(addr, item, elem_size);
	(*cur)++;
}


void* _dbuf_get( isize index, void *data, isize *cur, isize elem_size ) {
	dassert(index >= 0 && index < *cur, "[dbuf] Cannot access array out of bounds, index: %zd, dbuf current length: %zd.", index, *cur);
	dassert(data != NULL, "[dbuf] Data is NULL, possible memory corruption,");

	return (char*)data + (index * elem_size);
}


void _dbuf_set ( void * item, isize index, void *data, isize *cur, isize elem_size ) {
	dassert(index >= 0 && index < *cur, "[dbuf] Cannot access array out of bounds, index: %zd, dbuf current length: %zd.", index, *cur);
	dassert(data != NULL, "[dbuf] Data is NULL, possible memory corruption.");
	dassert(item != NULL, "[dbuf] Cannot insert NULL item into dbuf.");

	char *addr = (char*)data + (index * elem_size);
	memcpy(addr, item, elem_size);
}


void _dbuf_free ( void **data ) {
	d_free(data);
}
