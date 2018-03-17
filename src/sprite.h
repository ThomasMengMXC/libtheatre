#ifndef SPRITE_H
#define SPRITE_H

typedef enum {
	GRASS = 1,
	MOVE_RANGE,
	CURSOR,
} Colour;

typedef int (*Button)(void *);

typedef struct {
	char colourDepth, iconDepth, buttonDepth;
	Colour *colour;
	char **icon;
	Button *button;
} Sprite;

Sprite **init_sprite(short yLength, short xLength);
void free_sprite(short yLength, short xLength, Sprite **sprite);

#endif
