#ifndef SCREEN_H
#define SCREEN_H

#include "screen_struct.h"
#include "layer_struct.h"

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
