#ifndef SPRITE_STRUCT_H
#define SPRITE_STRUCT_H

#include "colour.h"

struct Props; struct Layer; // prototypes for the following two prototypes :^)
typedef int (*Button)(struct Props *, short y, short x);
typedef int (*Hover)(struct Props *, char start, short y, short x);

typedef struct Sprite {
	void *colour; // all of these are void pointers to c++ stacks
	void *icon;
	void *button;
	void *hover;
	void *attr;
} Sprite;

#ifdef __cplusplus
extern "C" {
#endif
Sprite **init_sprite(short yLength, short xLength);
void free_sprite(Sprite **sprite, short yLength, short xLength);
#ifdef __cplusplus
}
#endif

#endif
