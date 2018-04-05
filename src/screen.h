#ifndef SCREEN_H
#define SCREEN_H

#include "layer.h"

typedef struct {
	short depth, maxDepth;  // The amount of layers that there are
	short yLength, xLength; // Size of the screen
	Layer **layer; // The layers for the screen
	char **update; // Which tile should be drawn
} Screen;

int draw_screen(Screen *scr);

Screen *init_screen(short yLength, short xLength);
void free_screen(Screen *scr);

char **init_update_map(short yLength, short xLength);
void free_update_map(char **update, short yLength, short xLength);

Layer *add_layer_to_scr(Screen *scr, short yOffset, short xOffset,
		short yLength, short xLength);
void remove_layer_from_scr(Screen *scr);

Sprite **init_sprite(short yLength, short xLength);
void free_sprite(short yLength, short xLength, Sprite **sprite);

#endif
