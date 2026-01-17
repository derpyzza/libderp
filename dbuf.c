#include "derp.h"

#include <stdlib.h>
#include <string.h>

void _dbuf_make ( isize size, dbuf_data data ) {
	_dbuf_make_alloc ( size, def_allocator, data );
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

	memcpy( *data.data, *from, size * data.elem_size);
	(*data.cur) += size;
}

void _dbuf_grow ( dbuf_data dbuf ) {
	dassert(dbuf.alloc != NULL, "No allocator provided to dbuf, make sure to set an allocator for your dbuf");

	isize size;
	if ( *dbuf.len == 0) size = 32;
	else size = *dbuf.len * 2;
	*dbuf.len = size;

	*dbuf.data = dalloc_realloc((*dbuf.alloc), *dbuf.data, size * dbuf.elem_size);
}

void _dbuf_reserve(isize len, dbuf_data dbuf) {
	dassert(dbuf.alloc != NULL, "No allocator provided to dbuf, make sure to set an allocator for your dbuf");
	dassert(len > 0, "reserve length must not be zero or negative, %zd", len);

	*dbuf.data = dalloc_realloc( (*dbuf.alloc), *dbuf.data, len * dbuf.elem_size);
	*dbuf.len += len;
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
		_dbuf_grow(buf);
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
		_dbuf_grow(data);
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

void _dbuf_remove ( isize index, dbuf_data data ) {
	dassert(index >= 0 && index <= (*data.cur), "[dbuf] Cannot access array out of bounds, index: %zd, dbuf current length: %zd.", index, (*data.cur));
	dassert((data.data) != NULL, "[dbuf] Data is NULL, possible memory corruption.");
	dassert(data.alloc != NULL, "No allocator provided to dbuf, make sure to set an allocator for your dbuf");

	if(index < (*data.cur) - 1) {
		memmove(
		  (char*)(*data.data) + (index * data.elem_size)
		  , (char*)(*data.data) + ( (index + 1) * data.elem_size)
		  , ((*data.cur) - 1 - index) * data.elem_size
		);
	}

	(*data.cur)--;
}

void _dbuf_append ( void** data, isize len, dbuf_data dbuf ) {
	dassert((dbuf.data) != NULL, "[dbuf] Data is NULL, possible memory corruption.");
	dassert(data != NULL, "Input array must not be NULL");
	dassert(len > 0, "array length must not be zero or negative, %zd", len);

	if ( *dbuf.cur + len >= (*dbuf.len) ) {
		_dbuf_reserve(len, dbuf);
	}

	memcpy(
		(char*)(*dbuf.data) + ( (*dbuf.cur) * dbuf.elem_size),
		(char*)data,
		len * dbuf.elem_size
	);
	*dbuf.cur += len;
}

void _dbuf_prepend ( void** data, isize len, dbuf_data dbuf ) {
	dassert((dbuf.data) != NULL, "[dbuf] Data is NULL, possible memory corruption.");
	dassert(data != NULL, "Input array must not be NULL");
	dassert(len > 0, "array length must not be zero or negative, %zd", len);

	if ( *dbuf.cur + len >= (*dbuf.len) ) {
		_dbuf_reserve(len, dbuf);
	}

	memmove(
		(char*)(*dbuf.data) + ( len * dbuf.elem_size),
		(char*)(*dbuf.data),
		(*dbuf.cur) * dbuf.elem_size
	);

	memcpy(
		(char*)(*dbuf.data),
		(char*)data,
		len * dbuf.elem_size
	);

	*dbuf.cur += len;
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


void _dbuf_clone ( dbuf_data dest, dbuf src ) {
	dassert(src.data != NULL, "clone src is null");
	dassert(src.cur > 0 && src.len > 0, "src is empty");
	dassert(src.cur < src.len, "cur must not be greater than len");	

	*dest.cur = src.cur;
	*dest.len = src.len;
	*dest.alloc = *src.alloc;
	*dest.data = dalloc_alloc((*dest.alloc), *dest.len * src.elem_size);
	memcpy(*dest.data, src.data, *dest.cur * src.elem_size);
}


void _dbuf_free ( dbuf_data data ) {
	dalloc_free((*data.alloc), *data.data);
}
