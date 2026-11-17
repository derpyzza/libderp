#include "derp.h"

#include <stdlib.h>
#include <string.h>

void __populate_dbuf_data(dbuf_data * data, dbuf * n, isize elem_size) {
	n->elem_size = elem_size;
	data->data_ptr = (void**)&n->data;
	data->alloc = n->alloc;
	data->cur = &n->cur;
	data->len = &n->len;
	data->elem_size = elem_size;
}

void _dbuf_make ( isize size, dbuf_data data ) {
	_dbuf_make_alloc ( size, def_allocator, data );
}


void _dbuf_init ( const void **from, isize size, dbuf_data data ) {
	_dbuf_init_alloc ( from, size, def_allocator, data );
}


void _dbuf_make_alloc ( isize size, d_allocator alloc, dbuf_data data ) {
	dassert(size > 0, "[dbuf] Cannot make a dbuf of size %zd", size);

	data.alloc = alloc;
	(*data.cur) = 0;
	(*data.len) = size;
	(*data.data_ptr) = dalloc_calloc((data.alloc), (*data.len), data.elem_size);
	dassert(*data.data_ptr != NULL, "????");
	dassert(data.data_ptr != NULL, "huh?");
}


void _dbuf_init_alloc ( const void **from, isize size, d_allocator alloc, dbuf_data data ) {
	dassert(size > 0, "[dbuf] Cannot make a dbuf of size %zd", size);
	dassert(from != NULL, "[dbuf] Cannot copy NULL data");

	_dbuf_make_alloc(size, alloc, data);	

	memcpy( *data.data_ptr, *from, size * data.elem_size);
	(*data.cur) += size;
}

void _dbuf_grow ( dbuf_data data ) {
	isize size;
	if ( *data.len == 0) size = 32;
	else size = *data.len * 2;
	*data.len = size;

	*data.data_ptr = dalloc_realloc((data.alloc), *data.data_ptr, size * data.elem_size);
}

void _dbuf_reserve(isize len, dbuf_data data) {
	dassert(len > 0, "reserve length must not be zero or negative, %zd", len);

	isize new_len = *data.len + len;
	*data.data_ptr = dalloc_realloc((data.alloc), *data.data_ptr, new_len * data.elem_size);
	*data.len = new_len;
}


void _dbuf_push( void *item, dbuf_data data ) {
	dassert( (*data.cur) <= (*data.len)
	  , "[dbuf]: current item must not exceed dbuf length"
	);
	dassert(!( (*data.cur) > 0 && !(*data.data_ptr))
	  , "[dbuf]: possible memory corruption, data mustn't be null when cur is greater than 0"
	);
	dassert(data.elem_size > 0, "[dbuf] elem_size must not be zero or negative");

	if ( (*data.cur) >= (*data.len)) {
		_dbuf_grow(data);
	}

	char *addr = (char*)(*data.data_ptr) + ( (*data.cur) * data.elem_size);
	memcpy(addr, item, data.elem_size);
	(*data.cur)++;
}


void* _dbuf_pop ( dbuf_data data ) {
	dassert((*data.cur) > 0, "[dbuf] Cannot pop off a negative index");
	dassert(data.data_ptr != NULL, "[dbuf] Data is NULL, possible memory corruption");

	(*data.cur)--;
	return (char*)(*data.data_ptr) + ((*data.cur) * data.elem_size);
}


void _dbuf_insert ( void *item, isize index, dbuf_data data ) {
	dassert(index >= 0 && index <= (*data.cur), "[dbuf] Cannot access array out of bounds, index: %zd, dbuf current length: %zd.", index, (*data.cur));
	dassert((data.data_ptr) != NULL, "[dbuf] Data is NULL, possible memory corruption.");
	dassert(item != NULL, "[dbuf] Cannot insert NULL item into dbuf.");

	if ((*data.cur) >= (*data.len)) {
		_dbuf_grow(data);
	}

	if(index < (*data.cur)) {
		memmove(
		  (char*)(*data.data_ptr) + ((index + 1) * data.elem_size)
		  , (char*)(*data.data_ptr) + (index * data.elem_size)
		  , ((*data.cur) - index) * data.elem_size
		);
	}

	char *addr = (char*)(*data.data_ptr) + (index * data.elem_size);
	memcpy(addr, item, data.elem_size);
	(*data.cur)++;
}

void _dbuf_remove ( isize index, dbuf_data data ) {
	dassert(index >= 0 && index <= (*data.cur), "[dbuf] Cannot access array out of bounds, index: %zd, dbuf current length: %zd.", index, (*data.cur));
	dassert((data.data_ptr) != NULL, "[dbuf] Data is NULL, possible memory corruption.");

	if(index < (*data.cur) - 1) {
		memmove(
		  (char*)(*data.data_ptr) + (index * data.elem_size)
		  , (char*)(*data.data_ptr) + ( (index + 1) * data.elem_size)
		  , ((*data.cur) - 1 - index) * data.elem_size
		);
	}

	(*data.cur)--;
}

void _dbuf_append ( const void** data, isize len, dbuf_data dbuf ) {
	dassert((dbuf.data_ptr) != NULL, "[dbuf] Data is NULL, possible memory corruption.");
	dassert(data != NULL, "Input array must not be NULL");
	dassert(len > 0, "array length must not be zero or negative, %zd", len);

	if ( *dbuf.cur + len >= (*dbuf.len) ) {
		_dbuf_reserve(len, dbuf);
	}

	memcpy(
		(char*)(*dbuf.data_ptr) + ( (*dbuf.cur) * dbuf.elem_size),
		(char*)data,
		len * dbuf.elem_size
	);
	*dbuf.cur += len;
}

void _dbuf_prepend ( const void** data, isize len, dbuf_data dbuf ) {
	dassert((dbuf.data_ptr) != NULL, "[dbuf] Data is NULL, possible memory corruption.");
	dassert(data != NULL, "Input array must not be NULL");
	dassert(len > 0, "array length must not be zero or negative, %zd", len);

	if ( *dbuf.cur + len >= (*dbuf.len) ) {
		_dbuf_reserve(len, dbuf);
	}

	memmove(
		(char*)(*dbuf.data_ptr) + ( len * dbuf.elem_size),
		(char*)(*dbuf.data_ptr),
		(*dbuf.cur) * dbuf.elem_size
	);

	memcpy(
		(char*)(*dbuf.data_ptr),
		(char*)data,
		len * dbuf.elem_size
	);

	*dbuf.cur += len;
}


void* _dbuf_get ( isize index, dbuf_data data ) {
	dassert(index >= 0 && index < (*data.cur), "[dbuf] Cannot access array out of bounds, index: %zd, dbuf current length: %zd.", index, (*data.cur));
	dassert(data.data_ptr != NULL, "[dbuf] Data is NULL, possible memory corruption,");

	return (char*)(*data.data_ptr) + (index * data.elem_size);
}


void  _dbuf_set ( isize index, void *item, dbuf_data data ) {
	dassert(index >= 0 && index < (*data.cur), "[dbuf] Cannot access array out of bounds, index: %zd, dbuf current length: %zd.", index, (*data.cur));
	dassert(data.data_ptr != NULL, "[dbuf] Data is NULL, possible memory corruption.");
	dassert(item != NULL, "[dbuf] Cannot insert NULL item into dbuf.");

	char *addr = (char*)(*data.data_ptr) + (index * data.elem_size);
	memcpy(addr, item, data.elem_size);
}


void _dbuf_clone ( dbuf_data dest, dbuf src ) {
	dassert(src.data != NULL, "clone src is null");
	dassert(src.cur > 0 && src.len > 0, "src is empty");
	dassert(src.cur < src.len, "cur must not be greater than len");	

	*dest.cur   = src.cur;
	*dest.len   = src.len;
	 dest.alloc = src.alloc;
	*dest.data_ptr  = dalloc_alloc((dest.alloc), *dest.len * src.elem_size);
	memcpy(*dest.data_ptr, src.data, *dest.cur * src.elem_size);
}


void _dbuf_free ( dbuf_data data ) {
	dalloc_free((data.alloc), *data.data_ptr);
}
