#include <stack>
#include <vector>
#include "updateStack.h"

static char **init_char_grid(unsigned y, unsigned x);
static void free_char_grid(char **grid, unsigned y, unsigned x);

extern "C" {
UpdateStack *init_update_stack(unsigned y, unsigned x) {
	UpdateStack *update = new UpdateStack();
	update->stack = (void *) new std::stack<Vector2D>();
	update->grid = init_char_grid(y, x);
	update->yMax = y; update->xMax = x;
	return update;
}

void free_update_stack(UpdateStack *update, unsigned y, unsigned x) {
	delete (std::stack<Vector2D> *) update->stack;
	free_char_grid(update->grid, y, x);
	delete update;
}

void vector2D_push(UpdateStack *update, Vector2D vector) {
	auto tmp = (std::stack<Vector2D> *) update->stack;
	if (vector.y >= 0 && vector.y < update->yMax &&
			vector.x >= 0 && vector.x < update->xMax &&
			update->grid[vector.y][vector.x] == 0) {
		tmp->push(vector);
		update->grid[vector.y][vector.x] = 1;
	}
}

Vector2D vector2D_pop(UpdateStack *update) {
	Vector2D retVal; retVal.y = -1; retVal.x = -1;

	auto tmp = (std::stack<Vector2D> *) update->stack;
	if (!tmp->empty()) {
		retVal = tmp->top();
		tmp->pop();
		update->grid[retVal.y][retVal.x] = 0;
	}
	return retVal;
}
}

// static functions ------------------
static char **init_char_grid(unsigned y, unsigned x) {
	char **grid = new char*[y]();
	for (unsigned i = 0; i < y; i++) {
		grid[i] = new char[x]();
	}
	return grid;
}

static void free_char_grid(char **grid, unsigned y, unsigned x) {
	for (unsigned i = 0; i < y; i++) {
		delete[] grid[i];
	}
	delete[] grid;
}
