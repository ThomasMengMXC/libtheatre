#include <stdlib.h>
#include "updateStack.h"

static char **init_char_grid(unsigned y, unsigned x);
static void free_char_grid(char **grid, unsigned y, unsigned x);

UpdateStack *init_update_stack(unsigned y, unsigned x) {
	UpdateStack *update = malloc(sizeof(UpdateStack));
	update->stack = init_uVec("Update", sizeof(Vector2D));
	update->grid = init_char_grid(y, x);
	update->yMax = y; update->xMax = x;
	return update;
}

void free_update_stack(UpdateStack *update, unsigned y, unsigned x) {
	free_uVec(update->stack);
	free_char_grid(update->grid, y, x);
	free(update);
}

void vector2D_push(UpdateStack *update, Vector2D vector) {
	if (vector.y >= 0 && vector.y < update->yMax &&
			vector.x >= 0 && vector.x < update->xMax &&
			update->grid[vector.y][vector.x] == 0) {
		uVec_push(update->stack, &vector);
		update->grid[vector.y][vector.x] = 1;
	}
}

Vector2D vector2D_pop(UpdateStack *update) {
	Vector2D retVal; retVal.y = -1; retVal.x = -1;

	if (update->stack->length != 0) {
		retVal = *(Vector2D *) uVec_get_back(update->stack);
		uVec_pop(update->stack);
		update->grid[retVal.y][retVal.x] = 0;
	}
	return retVal;
}

// static functions ------------------
static char **init_char_grid(unsigned y, unsigned x) {
	char **grid = malloc(sizeof(char *) * y);
	for (unsigned i = 0; i < y; i++) {
		grid[i] = calloc(sizeof(char), x);
	}
	return grid;
}

static void free_char_grid(char **grid, unsigned y, unsigned x) {
	for (unsigned i = 0; i < y; i++) {
		free(grid[i]);
	}
	free(grid);
}
