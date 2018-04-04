#include <ncurses.h>
#include <stdlib.h>
#include "screen.h"
#include "layer.h"

int draw_screen(Screen *scr) {
	short colourLayer, iconLayer;
	for (short y = 0; y < scr->yLength; y++) {
		for (short x = 0; x < scr->xLength; x++) {
			colourLayer = iconLayer = scr->depth;
			while (activate_colour(scr->layer, y, x, colourLayer)) {
				--colourLayer;
			}
			while (draw_icon(scr->layer, y, x, iconLayer)) {
				--iconLayer;
			}
			if (iconLayer < 1) {
				mvprintw(y, 2 * x, "  ");
			}
			deactivate_colour(scr->layer, y, x, colourLayer);
		}
	}
	refresh();
	return 0;
}

Screen *init_screen(short yLength, short xLength) {
	Screen *scr = malloc(sizeof(Screen));
	scr->depth = 0; scr->maxDepth = 0;
	if (yLength == 0 && xLength == 0) {
		getmaxyx(stdscr, yLength, xLength);
	}
	scr->yLength = yLength;
	scr->xLength = xLength;
	scr->layer = NULL;
	scr->update = init_update_map(scr->yLength, scr->xLength);
	return scr;
}

void free_screen(Screen *scr) {
	while (scr->depth) {
		remove_layer_from_scr(scr);
	}
	free_update_map(scr->update, scr->yLength, scr->xLength);
	free(scr);
}

char **init_update_map(short yLength, short xLength) {
	char **update = malloc(sizeof(char *) * yLength);
	for (short y = 0; y < yLength; y++) {
		update[y] = malloc(sizeof(char) * xLength);
		for (short x = 0; x < xLength; x++) {
			update[y][x] = 0;
		}
	}
	return update;
}

void free_update_map(char **update, short yLength, short xLength) {
	for (short y = 0; y < yLength; y++) {
		free(update[y]);
	}
	free(update);
	return;
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
	scr->layer[scr->depth - 1]->update = &(scr->update);
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
