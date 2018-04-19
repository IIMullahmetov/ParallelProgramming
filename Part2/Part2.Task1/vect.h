#ifndef VECTOR_H__
#define VECTOR_H__

typedef struct vector_ {
	void** data;
	int size;
	int count;
} vect;

void vector_init(vect*);
int vector_count(vect*);
void vector_add(vect*, void*);
void vector_set(vect*, int, void*);
void *vector_get(vect*, int);
void vector_delete(vect*, int);
void vector_free(vect*);

#endif