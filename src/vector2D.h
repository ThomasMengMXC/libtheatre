#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <stdint.h>

typedef struct {
	uint16_t y, x;
} Vector2D;

typedef struct {
	Vector2D *vector2D;
	uint16_t depth, maxDepth;
	uint16_t yMax, xMax;
	char **grid;
} UpdateStack;

UpdateStack *init_update_stack(uint16_t y, uint16_t x);
void free_update_stack(UpdateStack *update, uint16_t y, uint16_t x);

char **init_vector2D_grid(uint16_t y, uint16_t x);
void free_vector2D_grid(char **grid, uint16_t y, uint16_t x);

void vector2D_push(UpdateStack *update, Vector2D vector);
Vector2D vector2D_pop(UpdateStack *update);

#endif
