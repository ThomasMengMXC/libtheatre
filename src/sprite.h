#ifndef SPRITE_H
#define SPRITE_H

#include "colour.h"
#include "uVec.h"

struct Props; struct Layer; // prototypes for the following two prototypes :^)
typedef int (*Button)(struct Props *, short y, short x);
typedef int (*Hover)(struct Props *, char start, short y, short x);

typedef struct Sprite {
	uVec *colour;
	uVec *icon;
	uVec *button;
	uVec *hover;
	uVec *attr;
} Sprite;

Sprite **init_sprite(short yLength, short xLength);
void free_sprite(Sprite **sprite, short yLength, short xLength);

#endif
