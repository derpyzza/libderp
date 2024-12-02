#pragma once

#include "dtypes.h"

// type-unsafe dynamic array
typedef struct Vec {
	size max;
	size current;
	void** data;
} Vec;

// evil preprocessor macro hack
// declares a type-safe dynamic array, along with all the functions required to push data into and pull data from it.
// T is the base type, I is the name for the outputted datatype ( eg for when you have a char* vec, but you want to call it a string vec )
#define DECL_VEC_TYPE(T, I) 																					\
	typedef struct Vec##I {				 																			\
		T *data; 																													\
		size current, max;																								\
	} Vec##I;																									          \
	static inline Vec##I *new_vec_##I (size init) {          						\
		Vec##I *v = (Vec##I*)malloc(sizeof(Vec##I));											\
		if (v) { 																													\
			v->max = init; 																									\
			v->current = 0;																									\
			v->data = ( T* )calloc(v->max, sizeof( T ));										\
		}																																	\
		return v;																													\
	}																																		\
																																			\
	static inline void vec_grow_##I (Vec##I *v, size by){ 	         		\
		if (v->current + 1 > v->max) {																		\
			v->max *= 2;																										\
			v->data = (T*)realloc(v->data, sizeof(T) * v->max);							\
		}																																	\
	}																																		\
																																			\
	static inline void vec_push_##I (Vec##I* v, T i) {	     						\
		if (v->current + 1 > v->max) { 																		\
			v->max *= 2; 																										\
			v->data = (T*)realloc(v->data, sizeof(T) * v->max); 						\
		} 																																\
		v->data[v->current] = i; 																					\
		v->current++; 																										\
	}																																		\
	static inline T *vec_pop_##I (Vec##I* v) {      	  								\
		return (T *)&v->data[(--v->current)];															\
	}

Vec *new_vec(size init, size elem_size);
Vec *init_vec(void** data, size len);
void vec_push(Vec *v, void* item);
void vec_pushi(Vec *v, int i);
void vec_pop(Vec *v);
