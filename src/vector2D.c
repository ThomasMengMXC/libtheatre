#include <stdlib.h>
#include <stdint.h>
#include "vector2D.h"

UpdateStack *init_update_stack(uint16_t y, uint16_t x) {
	UpdateStack *update = malloc(sizeof(UpdateStack));
	update->vector2D = NULL;
	update->grid = init_vector2D_grid(y, x);
	update->depth = 0; update->maxDepth = 0;
	update->yMax = y; update->xMax = x;
	return update;
}

void free_update_stack(UpdateStack *update, uint16_t y, uint16_t x) {
	free(update->vector2D);
	free_vector2D_grid(update->grid, y, x);
	free(update);
}

char **init_vector2D_grid(uint16_t y, uint16_t x) {
	char **grid = calloc(y, sizeof(char *));
	for (int i = 0; i < y; i++) {
		grid[i] = calloc(x, sizeof(char));
	}
	return grid;
}

void free_vector2D_grid(char **grid, uint16_t y, uint16_t x) {
	for (int i = 0; i < y; i++) {
		free(grid[i]);
	}
	free(grid);
}
	

void vector2D_push(UpdateStack *update, Vector2D vector) {
	if (vector.y >= 0 && vector.y < update->yMax &&
			vector.x >= 0 && vector.x < update->xMax &&
			update->grid[vector.y][vector.x] == 0) {
		update->depth++;
		if (update->depth > update->maxDepth) {
			update->maxDepth = update->depth * 10;
			update->vector2D = realloc(update->vector2D,
					sizeof(Vector2D) * update->maxDepth);
		}
		update->vector2D[update->depth - 1] = vector;
		update->grid[vector.y][vector.x] = 1;
	}
}

Vector2D vector2D_pop(UpdateStack *update) {
	Vector2D returnValue = {.y = -1, .x = -1};
	if (update->depth) {
		returnValue = update->vector2D[update->depth - 1];
		update->vector2D[update->depth - 1] = (Vector2D) {.y = -1, .x = -1};
		update->depth--;
		if (update->depth * 20 < update->maxDepth) {
			update->maxDepth = update->depth * 10;
			if (update->maxDepth == 0) {
				free(update->vector2D);
				update->vector2D = NULL;
			} else {
				update->vector2D = realloc(update->vector2D,
						sizeof(Vector2D) * update->maxDepth);
			}
		}
		update->grid[returnValue.y][returnValue.x] = 0;
	}
	return returnValue;
}

