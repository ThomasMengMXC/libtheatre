#include <stdlib.h>
#include <ncurses.h>
#include "sprite.h"

Sprite **init_sprite(short yLength, short xLength) {
	Sprite **spriteArray = malloc(sizeof(Sprite *) * yLength);
	for (int y = 0; y < yLength; y++) {
		spriteArray[y] = malloc(sizeof(Sprite) * xLength);
		for (int x = 0; x < xLength; x++) {
			Sprite *sprite = &(spriteArray[y][x]);
			sprite->colour = init_uVec(sizeof(Colour));
			sprite->icon = init_uVec(sizeof(char[3]));
			sprite->button = init_uVec(sizeof(Button));
			sprite->hover = init_uVec(sizeof(Hover));
			sprite->attr = init_uVec(sizeof(attr_t));
		}
	}
	return spriteArray;
}

void free_sprite(Sprite **sprite, short yLength, short xLength) {
	for (int y = 0; y < yLength; y++) {
		for (int x = 0; x < xLength; x++) {
			Sprite *tmp = &(sprite[y][x]);
			free_uVec(tmp->colour);
			free_uVec(tmp->icon);
			free_uVec(tmp->button);
			free_uVec(tmp->hover);
			free_uVec(tmp->attr);
		}
		free(sprite[y]);
	}
	free(sprite);
}
