#pragma once

#include "dtypes.h"

// type-unsafe dynamic array
typedef struct dvec {
	size cap         // Max allocated size	
		 , current     // Current item || length of the array
		 , elem_size;  // size of one individual element, for alignment purposes

	void** data;     // the data array itself
} dvec;

/**
	@param type — type of data
	@param size — initial array size 
*/
#define new_dvec(type, size) _new_dvec(size, sizeof(type))
#define init_dvec(type, size, data) _init_dvec(data, size, sizeof(type))

#define _dv_push(type, vec, item) dvec_##type_push(vec, item)

/**
	@param init — initial array size
	@param elem_size — size of one element
*/
dvec * _new_dvec(size init, size elem_size);

/** 
	@param data — Initial data to start out with
	@param len — length of array
*/
dvec *_init_dvec(void** data, size len, size elem_size);

/** 
	@param vec — vec to push to
	@param item — item to push
	@param size — size of individual element
*/
void dvec_push(dvec *v, void* item);

/** 
	@param v — vec to pop out from
*/
void* dvec_pop(dvec *v);

// evil preprocessor macro hack
// declares a type-safe dynamic array, along with all the functions required to push data into and pull data from it.
// T is the base type, I is the name for the outputted datatype ( eg for when you have a char* vec, but you want to call it a string vec )
#define DECL_VEC_TYPE(T, I)                                            \
	typedef struct dvec_##I {                                            \
		T *data;                                                           \
		size current, max;                                                 \
	} dvec_##I;                                                          \
	static inline dvec_##I *new_dvec_##I (size init) {                   \
		dvec_##I *v = (dvec_##I*)malloc(sizeof(dvec_##I));                 \
		if (v) {                                                           \
			v->max = init;                                                   \
			v->current = 0;                                                  \
			v->data = ( T* )calloc(v->max, sizeof( T ));                     \
		}                                                                  \
		return v;                                                          \
	}                                                                    \
	static inline void dvec_grow_##I (dvec_##I *v){             \
		if (v->current + 1 > v->max) {                                     \
			v->max *= 2;                                                     \
			v->data = (T*)realloc(v->data, sizeof(T) * v->max);              \
		}                                                                  \
	}                                                                    \
	static inline void dvec_push_##I (dvec_##I* v, T i) {                \
		if (v->current + 1 > v->max) {                                     \
			v->max *= 2;                                                     \
			v->data = (T*)realloc(v->data, sizeof(T) * v->max);              \
		}                                                                  \
		v->data[v->current] = i;                                           \
		v->current++;                                                      \
	}                                                                    \
	static inline T *vec_pop_##I (dvec_##I* v) {                         \
		return (T *)&v->data[(--v->current)];                              \
	}
