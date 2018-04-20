#ifndef SPRITE_STRUCT_H
#define SPRITE_STRUCT_H

#include <stdint.h>

struct Props; struct Layer; // prototypes for the following two prototypes :^)
typedef int (*Button)(struct Props *, struct Layer *, short y, short x);
typedef int (*Hover)(struct Props *, struct Layer *, char start, short y, short x);

typedef struct Sprite {
	char colourDepth, iconDepth, buttonDepth, hoverDepth;
	char colourMaxDepth, iconMaxDepth, buttonMaxDepth, hoverMaxDepth;
	uint8_t *colour;
	char (*icon)[3];
	Button *button;
	Hover *hover;
} Sprite;

Sprite **init_sprite(short yLength, short xLength);
void free_sprite(Sprite **sprite, short yLength, short xLength);

#endif
