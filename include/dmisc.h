#pragma once

// i'm defining this jic but idk if i'm gonna end up using this
typedef struct Result {
	int err;
	void *ok;
} Result;

// wrapper for a static-sized array
#define ARRAY(size, I) \
struct Arr##I { \
	size len; \
	T* data; \
}

#define CREATE_ARRAY_TYPE(T, I) \
	typedef struct Arr##I {\
		size len;		\
		T* data; \
		T* (*get)(size id); \
	}Arr##I;\
	Arr##I *new_array_##T(size size) {\
		Arr##I *a = malloc(sizeof(*a));\
		a->len = size;\
		a->data = (T*)malloc(sizeof(T) * size); \
		return a;\
	}
