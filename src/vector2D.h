#ifndef VECTOR2D_H
#define VECTOR2D_H

typedef struct {
	short y, x;
} Vector2D;

typedef struct {
	Vector2D *vector2D;
	unsigned int depth, maxDepth;
} Vector2DStack;

Vector2DStack *init_vector2D_stack(void);
void free_vector2D_stack(Vector2DStack *stack);

void vector2D_push(Vector2DStack *stack, Vector2D vector);
Vector2D vector2D_pop(Vector2DStack *stack);

#endif
