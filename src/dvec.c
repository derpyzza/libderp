#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dvec.h"
#include "ddebug.h"

dvec *_new_dvec(size init, size elem_size) {
	dvec *v = (dvec*)malloc(sizeof(dvec));
	if(v) {
		v->cap = init;
		v->current = 0;
		v->data = calloc(v->cap, sizeof(elem_size));
		return v;
	}

	else printf("Error: Not enough memory to create new vec\n"), exit(-1);
}

dvec *_init_dvec(void** data, size len, size elem_size) {
	dvec *v = _new_dvec(len, sizeof(elem_size));
	memcpy(data, v->data, elem_size * len);

	if (v) {
		return v;	
	} else {
		dlog_error("Couldn't create new vec, copy fail");
		return NULL;
	}
}

void dvec_push(dvec *v, void* item) {
	if (v->current + 1 > v->cap) {
		v->cap *= 2;
		v->data = realloc(v->data, sizeof(v->elem_size) * v->cap);
	}

	v->data[v->current] = item;
	v->current++;
}

void* dvec_pop(dvec *v) {
	if (v->current == 0) {
		return 0;
	}

	return v->data[--v->current];
}
