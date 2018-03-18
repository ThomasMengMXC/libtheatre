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

Sprite **init_sprite(short yLength, short xLength);
void free_sprite(short yLength, short xLength, Sprite **sprite);

#endif
