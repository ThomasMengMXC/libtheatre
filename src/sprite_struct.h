#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

typedef int (*Button)(void *);

typedef struct RGB {
	uint8_t term, r, g, b;
} RGB;

typedef struct {
	char colourDepth, iconDepth, buttonDepth;
	RGB *colour;
	char **icon;
	Button *button;
} Sprite;

#endif
