#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

typedef int (*Button)(void *, short y, short x);

typedef struct RGB {
	short term;
	uint8_t r, g, b;
} RGB;

typedef struct {
	char colourDepth, iconDepth, buttonDepth;
	char colourMaxDepth, iconMaxDepth, buttonMaxDepth;
	RGB *colour;
	char **icon;
	Button *button;
} Sprite;

#endif
