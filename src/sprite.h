#ifndef SPRITE_STRUCT_H
#define SPRITE_STRUCT_H

#include <stdint.h>

typedef int (*Button)(void *, short y, short x);

typedef struct {
	char colourDepth, iconDepth, buttonDepth;
	char colourMaxDepth, iconMaxDepth, buttonMaxDepth;
	char *colour;
	char **icon;
	Button *button;
} Sprite;

Sprite **init_sprite(short yLength, short xLength);
void free_sprite(short yLength, short xLength, Sprite **sprite);

#endif
