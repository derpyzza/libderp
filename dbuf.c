#include "derp.h"

#include <stdlib.h>

void _dbuf_make ( isize size, dbuf_data data ) {
	_dbuf_make_alloc ( size, def_allocator, data );
	dassert(*data.data != NULL, "HUH?");
}


void _dbuf_init ( void **from, isize size, dbuf_data data ) {
	_dbuf_init_alloc ( from, size, def_allocator, data );
}


void _dbuf_make_alloc ( isize size, d_allocator alloc, dbuf_data data ) {
	dassert(size > 0, "[dbuf] Cannot make a dbuf of size %zd", size);

	(*data.alloc) = alloc;
	(*data.cur) = 0;
	(*data.len) = size;
	(*data.data) = dalloc_calloc((*data.alloc), (*data.len), data.elem_size);
	dassert(*data.data != NULL, "????");
}


void _dbuf_init_alloc ( void **from, isize size, d_allocator alloc, dbuf_data data ) {
	dassert(size > 0, "[dbuf] Cannot make a dbuf of size %zd", size);
	dassert(from != NULL, "[dbuf] Cannot copy NULL data");

	_dbuf_make_alloc(size, alloc, data);	

	dassert(*data.data != NULL, "WHA?");

	memcpy( *data.data, *from, size * data.elem_size);
	(*data.cur) += size;
}


void _dbuf_push( void *item, dbuf_data buf ) {
	dassert( (*buf.cur) <= (*buf.len)
	  , "[dbuf]: current item must not exceed dbuf length"
	);
	dassert(!( (*buf.cur) > 0 && !(*buf.data))
	  , "[dbuf]: possible memory corruption, data mustn't be null when cur is greater than 0"
	);
	dassert(buf.elem_size > 0, "[dbuf] elem_size must not be zero or negative");
	dassert(buf.alloc != NULL, "No allocator provided to dbuf, make sure to set an allocator for your dbuf");

	if ( (*buf.cur) >= (*buf.len)) {
		int size;
		if ( *buf.len == 0) size = 32;
		else size = *buf.len * 2;
		*buf.len = size;

		*buf.data = dalloc_realloc((*buf.alloc), *buf.data, size * buf.elem_size);
	}

	char *addr = (char*)(*buf.data) + ( (*buf.cur) * buf.elem_size);
	memcpy(addr, item, buf.elem_size);
	(*buf.cur)++;
}


void* _dbuf_pop ( dbuf_data data ) {
	dassert((*data.cur) > 0, "[dbuf] Cannot pop off a negative index");
	dassert(data.data != NULL, "[dbuf] Data is NULL, possible memory corruption");

	(*data.cur)--;
	return (char*)(*data.data) + ((*data.cur) * data.elem_size);
}


void _dbuf_insert ( void *item, isize index, dbuf_data data ) {
	dassert(index >= 0 && index <= (*data.cur), "[dbuf] Cannot access array out of bounds, index: %zd, dbuf current length: %zd.", index, (*data.cur));
	dassert((data.data) != NULL, "[dbuf] Data is NULL, possible memory corruption.");
	dassert(item != NULL, "[dbuf] Cannot insert NULL item into dbuf.");
	dassert(data.alloc != NULL, "No allocator provided to dbuf, make sure to set an allocator for your dbuf");

	if ((*data.cur) >= (*data.len)) {
		int size;
		if ((*data.len) == 0) size = 32;
		else size = (*data.len) * 2;
		(*data.len) = size;

		(*data.data) = dalloc_realloc((*data.alloc), (*data.data), size * data.elem_size);
	}

	if(index < (*data.cur)) {
		memmove(
		  (char*)(*data.data) + ((index + 1) * data.elem_size)
		  , (char*)(*data.data) + (index * data.elem_size)
		  , ((*data.cur) - index) * data.elem_size
		);
	}

	char *addr = (char*)(*data.data) + (index * data.elem_size);
	memcpy(addr, item, data.elem_size);
	(*data.cur)++;
}


void* _dbuf_get ( isize index, dbuf_data data ) {
	dassert(index >= 0 && index < (*data.cur), "[dbuf] Cannot access array out of bounds, index: %zd, dbuf current length: %zd.", index, (*data.cur));
	dassert(data.data != NULL, "[dbuf] Data is NULL, possible memory corruption,");

	return (char*)(*data.data) + (index * data.elem_size);
}


void  _dbuf_set ( isize index, void *item, dbuf_data data ) {
	dassert(index >= 0 && index < (*data.cur), "[dbuf] Cannot access array out of bounds, index: %zd, dbuf current length: %zd.", index, (*data.cur));
	dassert(data.data != NULL, "[dbuf] Data is NULL, possible memory corruption.");
	dassert(item != NULL, "[dbuf] Cannot insert NULL item into dbuf.");

	char *addr = (char*)(*data.data) + (index * data.elem_size);
	memcpy(addr, item, data.elem_size);
}


void _dbuf_free ( dbuf_data data ) {
	dalloc_free((*data.alloc), *data.data);
}
