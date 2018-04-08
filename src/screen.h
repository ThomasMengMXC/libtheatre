#ifndef SCREEN_H
#define SCREEN_H

#include "layer.h"
#include "vector2D.h"

typedef struct {
	unsigned short depth, maxDepth;  // The amount of layers that there are
	short yLength, xLength; // Size of the screen
	Layer **layer; // The layers for the screen
	Vector2DStack *update; // The stack for tiles to be drawn
} Screen;

Screen *init_screen(short yLength, short xLength);
void free_screen(Screen *scr);

int draw_screen(Screen *scr);

Layer *add_layer_to_scr(Screen *scr, short yOffset, short xOffset,
		short yLength, short xLength);
void remove_layer_from_scr(Screen *scr);

#endif
