#ifndef VECTOR2D_H
#define VECTOR2D_H

typedef struct Vector2D {
	unsigned y, x;
} Vector2D;

typedef struct UpdateStack {
	void *stack;
	unsigned yMax, xMax;
	char **grid;
} UpdateStack;

#ifdef __cplusplus
extern "C" {
#endif
UpdateStack *init_update_stack(unsigned y, unsigned x);
void free_update_stack(UpdateStack *update, unsigned y, unsigned x);

void vector2D_push(UpdateStack *update, Vector2D pos);
Vector2D vector2D_pop(UpdateStack *update);
#ifdef __cplusplus
}
#endif

#endif
