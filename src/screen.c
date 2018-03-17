#include <ncurses.h>
#include <stdlib.h>
#include "screen.h"
#include "layer.h"

extern FILE *fp;

int draw_screen(Screen *scr) {
	short colourLayer, iconLayer;
	if (!should_draw(scr->layer, scr->depth)) {
		return 0;
	}
	for (short y = 0; y < scr->yLength; y++) {
		for (short x = 0; x < scr->xLength; x++) {
			colourLayer = iconLayer = scr->depth;
			while (activate_colour(scr->layer, y, x, colourLayer)) {
				--colourLayer;
			}
			if (colourLayer < 1) {
				mvprintw(y, 2 * x, "  ");
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

// Are any layers worth drawing?
bool should_draw(Layer **layer, short depth) {
	while (depth > 0 && layer[depth - 1]->draw == false) {
		depth--;
	}
	if (depth < 1) {
		return false;
	}
	return true;
}

Screen *init_screen(short yLength, short xLength) {
	Screen *scr = malloc(sizeof(Screen));
	scr->depth = 0;
	scr->yLength = yLength;
	scr->xLength = xLength;
	scr->layer = NULL;
	return scr;
}

void free_screen(Screen *scr) {
	free(scr);
}

// creates a new layer on the screen and returns a pointer to said new layer
Layer *add_layer_to_scr(Screen *scr, short yOffset, short xOffset,
		short yLength, short xLength) {
	scr->depth++;
	scr->layer = realloc(scr->layer, sizeof(Layer *) * scr->depth);
	scr->layer[scr->depth - 1] = init_layer(yOffset, xOffset, yLength, xLength);
	return scr->layer[scr->depth - 1];
}

void remove_layer_from_scr(Screen *scr) {
	free_layer(scr->layer[scr->depth - 1]);
	
	if (scr->depth > 1) {
		scr->depth--;
		scr->layer = realloc(scr->layer, sizeof(Layer *) * scr->depth);
	} else if (scr->depth == 1) {
		scr->depth--;
		free(scr->layer);
		scr->layer = NULL;
	}
	return;
}
