#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <stdint.h>

typedef struct Vector2D {
	unsigned y, x;
} Vector2D;

typedef struct UpdateStack {
	Vector2D *vector2D;
	unsigned depth, maxDepth;
	unsigned yMax, xMax;
	char **grid;
} UpdateStack;

UpdateStack *init_update_stack(unsigned y, unsigned x);
void free_update_stack(UpdateStack *update, unsigned y, unsigned x);

char **init_vector2D_grid(unsigned y, unsigned x);
void free_vector2D_grid(char **grid, unsigned y, unsigned x);

void vector2D_push(UpdateStack *update, Vector2D vector);
Vector2D vector2D_pop(UpdateStack *update);

#endif
