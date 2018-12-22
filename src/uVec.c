#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "uVec.h"

uVec *init_uVec(unsigned unitSize) {
	uVec *u = malloc(sizeof(uVec));
	u->unitSize = unitSize;
	u->length = 0;
	u->maxLength = 1;
	u->array = malloc(u->maxLength * u->unitSize);
	return u;
}

void free_uVec(uVec *uVec) {
	free(uVec->array);
	free(uVec);
}


void uVec_push(uVec *uVec, void *unit) {
	uVec->length++;
	if (uVec->length >= uVec->maxLength) {
		uVec->maxLength = uVec->maxLength * 3 / 2 + 1;
		uVec->array = realloc(uVec->array, uVec->maxLength * uVec->unitSize);
	}
	memcpy((uint8_t *) uVec->array + (uVec->length - 1) * uVec->unitSize,
			unit, uVec->unitSize);
	return;
}

void uVec_pop(uVec *uVec) {
	if (uVec->length == 0) 
		return;
	uVec->length--;
	if (uVec->length < (uVec->maxLength + 1) / 2) {
		uVec->maxLength = (uVec->maxLength + 1) / 2;
		uVec->array = realloc(uVec->array, uVec->maxLength * uVec->unitSize);
	}
}

void *uVec_get(uVec *uVec, unsigned index) {
	if (index >= uVec->length)
		return NULL;
	return (uint8_t *) uVec->array + index * uVec->unitSize;
}

void uVec_replace(uVec *uVec, unsigned index, void *unit) {
	if (index >= uVec->length)
		return;
	memcpy((uint8_t *)uVec->array + index * uVec->unitSize,
			unit, uVec->unitSize);
}

void *uVec_get_back(uVec *uVec) {
	if (uVec->length == 0) 
		return NULL;
	return (uint8_t *) uVec->array + (uVec->length - 1) * uVec->unitSize;
}

