#ifndef SPRITE_STRUCT_H
#define SPRITE_STRUCT_H

#include "colour.h"

struct Props; struct Layer; // prototypes for the following two prototypes :^)
typedef int (*Button)(struct Props *, short y, short x);
typedef int (*Hover)(struct Props *, char start, short y, short x);

typedef struct Sprite {
	uint16_t colourDepth, iconDepth, buttonDepth, hoverDepth;
	uint16_t colourMaxDepth, iconMaxDepth, buttonMaxDepth, hoverMaxDepth;
	Colour *colour;
	char (*icon)[3];
	Button *button;
	Hover *hover;
} Sprite;

Sprite **init_sprite(short yLength, short xLength);
void free_sprite(Sprite **sprite, short yLength, short xLength);

#endif
