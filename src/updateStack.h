#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "uVec.h"
#include "coordinate.h"

typedef struct UpdateStack {
	uVec *stack;
	unsigned yMax, xMax;
	char **grid;
} UpdateStack;

UpdateStack *init_update_stack(unsigned y, unsigned x);
void free_update_stack(UpdateStack *update, unsigned y, unsigned x);

void vector2D_push(UpdateStack *update, Coordinate pos);
Coordinate vector2D_pop(UpdateStack *update);

#endif
