#ifndef SPRITE_H
#define SPRITE_H

typedef int (*Button)(void *);

typedef struct {
	char colourDepth, iconDepth, buttonDepth;
	char *colour;
	char **icon;
	Button *button;
} Sprite;

Sprite **init_sprite(short yLength, short xLength);
void free_sprite(short yLength, short xLength, Sprite **sprite);

#endif
