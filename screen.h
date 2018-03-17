#ifndef SCREEN_H
#define SCREEN_H
#include "layer.h"

typedef struct {
	short depth;
	short yLength, xLength;
	Layer **layer;
} Screen;

int draw_screen(Screen *scr);
bool should_draw(Layer **layer, short depth);

Screen *init_screen(short yLength, short xLength);
void free_screen(Screen *scr);

Layer *add_layer_to_scr(Screen *scr, short yOffset, short xOffset,
		short yLength, short xLength);
void remove_layer_from_scr(Screen *scr);

Sprite **init_sprite(short yLength, short xLength);
void free_sprite(short yLength, short xLength, Sprite **sprite);

#endif
