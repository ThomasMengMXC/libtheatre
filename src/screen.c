#include <curses.h>
#include <stdlib.h>

#include "screen.h"

Screen *init_screen(short yLength, short xLength) {
	Screen *scr = malloc(sizeof(Screen));
	scr->depth = 0; scr->maxDepth = 0;
	if (yLength == 0) yLength = getmaxy(stdscr) | LINES;
	if (xLength == 0) xLength = (getmaxx(stdscr) | COLS) / 2;
	scr->yLength = yLength; scr->xLength = xLength;
	scr->layer = NULL;
	scr->update = init_update_stack(yLength, xLength);
	scr->cursor = init_cursor();
	return scr;
}

void free_screen(Screen *scr) {
	free_cursor(scr->cursor);
	while (scr->depth) {
		remove_layer_from_scr(scr);
	}
	free_update_stack(scr->update, scr->yLength, scr->xLength);
	free(scr);
}

int draw_screen(Screen *scr) {
	short colourLayer, iconLayer, y, x, update;
	Vector2D vector;
	update = 0;
	while (scr->update->depth > 0) {
		update = 1;
		vector = vector2D_pop(scr->update);
		y = vector.y; x = vector.x;
		colourLayer = iconLayer = scr->depth;

		// go the colour layers until you reach a colour
		while (paint_colour(scr->layer, y, x, colourLayer)) --colourLayer;
		if (colourLayer < 1) attron(COLOR_PAIR(1));

		// go the icon layers until you reach an icon
		while (draw_icon(scr->layer, y, x, iconLayer)) --iconLayer;
		if (iconLayer < 1) mvprintw(y, 2 * x, "  ");
	}
	if (update) refresh();
	return 0;
}

// creates a new layer on the screen and returns a pointer to said new layer
Layer *add_layer_to_scr(Screen *scr, short yOffset, short xOffset,
		short yLength, short xLength) {
	scr->depth++;
	if (scr->depth >= scr->maxDepth) {
		scr->maxDepth = scr->depth * 2;
		scr->layer = realloc(scr->layer, sizeof(Layer *) * scr->maxDepth);
	}

	if (yLength == 0) yLength = scr->yLength;
	if (xLength == 0) xLength = scr->xLength;
	scr->layer[scr->depth - 1] = init_layer(yOffset, xOffset, yLength, xLength);
	scr->layer[scr->depth - 1]->update = scr->update;
	return scr->layer[scr->depth - 1];
}

void remove_layer_from_scr(Screen *scr) {
	if (scr->depth) {
		free_layer(scr->layer[scr->depth - 1]);
		scr->layer[scr->depth - 1] = NULL;
		scr->depth--;
		while (scr->depth * 2 < scr->maxDepth) {
			scr->maxDepth = scr->depth * 3 / 2;
			if (scr->maxDepth == 0) {
				free(scr->layer);
				scr->layer = NULL;
			} else {
				scr->layer = realloc(scr->layer,
						sizeof(Layer *) * scr->maxDepth);
			}
		}
	}
	return;
}
