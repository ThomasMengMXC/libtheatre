#ifndef SCREEN_H
#define SCREEN_H

#include "layer.h"
#include "vector2D.h"
#include "cursor.h"

typedef struct Screen {
	unsigned depth, maxDepth;  // The amount of layers that there are
	unsigned yLength, xLength; // Size of the screen
	Layer **layer; // The layers for the screen
	UpdateStack *update; // The stack for tiles to be drawn
	Cursor *cursor; // The cursor for interacting with the screen
} Screen;

Screen *init_screen(short yLength, short xLength);
void free_screen(Screen *scr);

int draw_screen(Screen *scr);

void clear_screen(Screen *scr);
void resize_screen(Screen *scr);

Layer *add_layer_to_scr(Screen *scr, short yOffset, short xOffset,
		short yLength, short xLength);
Layer *remove_layer_from_scr(Screen *scr);

Button get_button(Screen *scr, short y, short x);
Hover get_hover(Screen *scr, short y, short x);

void mv_cursor_relative(Screen *screen, short y, short x);
void mv_cursor_absolute(Screen *screen, short y, short x);

#endif
