#include <stdlib.h>
#include "sprite.h"

Sprite **init_sprite(short yLength, short xLength) {
	int y, x;
	Sprite **spriteArray = malloc(sizeof(Sprite *) * yLength);
	for (y = 0; y < yLength; y++) {
		spriteArray[y] = malloc(sizeof(Sprite) * xLength);
		for (x = 0; x < xLength; x++) {
			Sprite *sprite = &(spriteArray[y][x]);
			sprite->colour = NULL;
			sprite->icon = NULL;
			sprite->button = NULL;
			sprite->hover = NULL;

			sprite->colourDepth = 0;	sprite->colourMaxDepth = 0;
			sprite->iconDepth = 0;		sprite->iconMaxDepth = 0;
			sprite->buttonDepth = 0;	sprite->buttonMaxDepth = 0;
			sprite->hoverDepth = 0;		sprite->hoverMaxDepth = 0;
		}
	}
	return spriteArray;
}

void free_sprite(Sprite **sprite, short yLength, short xLength) {
	int y, x;
	for (y = 0; y < yLength; y++) {
		for (x = 0; x < xLength; x++) {
			if (sprite[y][x].icon != NULL) {
				free(sprite[y][x].icon);
			}
			if (sprite[y][x].colour != NULL) {
				free(sprite[y][x].colour);
			}
			if (sprite[y][x].button != NULL) {
				free(sprite[y][x].button);
			}
			if (sprite[y][x].hover != NULL) {
				free(sprite[y][x].hover);
			}
		}
		free(sprite[y]);
	}
	free(sprite);
}
