#ifndef UVECTOR_H
#define UVECTOR_H

typedef struct uVec {
	char name[20];
	unsigned unitSize;
	unsigned length;
	unsigned maxLength;
	void *array;
} uVec;

uVec *init_uVec(char *name, unsigned unitSize);
void free_uVec(uVec *vec);

void uVec_push(uVec *uVec, void *unit);
void uVec_pop(uVec *uVec);
void *uVec_get(uVec *uVec, unsigned index);
void uVec_replace(uVec *uVec, unsigned index, void *unit);
void *uVec_get_back(uVec *uVec);

#endif
