#include <stdlib.h>
#include "vector2D.h"

Vector2DStack *init_vector2D_stack(void) {
	Vector2DStack *stack = malloc(sizeof(Vector2DStack));
	stack->vector2D = NULL;
	stack->depth = 0;
	stack->maxDepth = 0;
	return stack;
}

void free_vector2D_stack(Vector2DStack *stack) {
	while (stack->depth) {
		vector2D_pop(stack);
	}
	free(stack);
}

void vector2D_push(Vector2DStack *stack, Vector2D vector) {
	stack->depth++;
	if (stack->depth >= stack->maxDepth) {
		stack->maxDepth = stack->depth * 2;
		stack->vector2D = realloc(stack->vector2D,
				sizeof(Vector2D *) * stack->maxDepth);
	}
	stack->vector2D[stack->depth - 1] = vector;
	return;
}

Vector2D vector2D_pop(Vector2DStack *stack) {
	Vector2D returnValue = {.y = -1, .x = -1};
	if (stack->depth) {
		returnValue = stack->vector2D[stack->depth - 1];
		stack->vector2D[stack->depth - 1] = (Vector2D) {.y = -1, .x = -1};
		stack->depth--;
		while (stack->depth * 2 < stack->maxDepth) {
			stack->maxDepth = stack->depth * 3 / 2;
			if (stack->maxDepth == 0) {
				free(stack->vector2D);
				stack->vector2D = NULL;
			} else {
				stack->vector2D = realloc(stack->vector2D,
						sizeof(Vector2D *) * stack->maxDepth);
			}
		}
	}
	return returnValue;
}

