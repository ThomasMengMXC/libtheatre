#include <ncurses.h>
#include <stdlib.h>

#include "screen.h"

Screen *init_screen(short yLength, short xLength) {
	Screen *scr = malloc(sizeof(Screen));
	scr->depth = 0; scr->maxDepth = 0;
	if (yLength == 0) yLength = getmaxy(stdscr);
	if (xLength == 0) xLength = getmaxx(stdscr);
	scr->yLength = yLength;
	scr->xLength = xLength;
	scr->layer = NULL;
	scr->update = init_vector2D_stack();
	return scr;
}

void free_screen(Screen *scr) {
	while (scr->depth) {
		remove_layer_from_scr(scr);
	}
	free_vector2D_stack(scr->update);
	free(scr);
}

int draw_screen(Screen *scr) {
	short colourLayer, iconLayer;
	Vector2D vector;
	while ((vector = vector2D_pop(scr->update)).y != -1) {
		short y = vector.y;
		short x = vector.x;
		colourLayer = iconLayer = scr->depth;
		// go the colour layers until you reach a colour
		while (activate_colour(scr->layer, y, x, colourLayer)) {
			--colourLayer;
		}
		// go the icon layers until you reach an icon
		while (draw_icon(scr->layer, y, x, iconLayer)) {
			--iconLayer;
		}
		// print a blank character if nothing is here
		// this should only happen when all icons have been removed
		if (iconLayer < 1) {
			mvprintw(y, 2 * x, "  ");
		}
		// deactivate the colour (essentially lift your pen up)
		deactivate_colour(scr->layer, y, x, colourLayer);
		
	}
	refresh();
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
