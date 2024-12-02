#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/dvec.h"

/**
	@param init — initial array size
	@param elem_size — size of one element
*/
Vec *new_vec(size init, size elem_size) {
	Vec *v = (Vec*)malloc(sizeof(Vec));
	if(v) {
		v->max = init;
		v->current = 0;
		v->data = (void**)calloc(v->max, sizeof(elem_size));
		return v;
	}

	else printf("Error: Not enough memory to create new vec\n"), exit(-1);
}

/** 
	@param data — Initial data to start out with
	@param len — length of array
*/
Vec *init_vec(void** data, size len) {
	Vec *v = new_vec(len, sizeof(void*));
	memcpy(data, v->data, len);
	return v;
}

void vec_push(Vec *v, void* item) {
	if (v->current + 1 > v->max) {
		v->max *= 2;
		v->data = (void**)realloc(v->data, sizeof(void*) * v->max);
	}

	v->data[v->current] = item;
	v->current++;
}

void vec_pushi(Vec *v, int i) {
	vec_push(v, (void *)(size)i);
}

void _vec_pop(Vec *v){
	if (v->current == 0) {
		return;
	}

	v->data[--v->current] = NULL;
	return;
}
