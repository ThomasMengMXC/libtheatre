#ifndef SPRITE_STRUCT_H
#define SPRITE_STRUCT_H

#include <stdint.h>

typedef int (*Button)(void *args, short y, short x);
typedef int (*Hover)(void *args, char start, short y, short x);

typedef struct {
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
