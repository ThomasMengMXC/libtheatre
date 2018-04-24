#ifndef SCREEN_H
#define SCREEN_H

#include "layer.h"
#include "vector2D.h"
#include "cursor.h"

typedef struct Screen {
	uint16_t depth, maxDepth;  // The amount of layers that there are
	uint16_t yLength, xLength; // Size of the screen
	Layer **layer; // The layers for the screen
	UpdateStack *update; // The stack for tiles to be drawn
	Cursor *cursor; // The cursor for interacting with the screen
} Screen;

Screen *init_screen(short yLength, short xLength);
void free_screen(Screen *scr);

int draw_screen(Screen *scr);

Layer *add_layer_to_scr(Screen *scr, short yOffset, short xOffset,
		short yLength, short xLength);
Layer *remove_layer_from_scr(Screen *scr);

void paint_colour(Screen *scr, short y, short x);

#endif
