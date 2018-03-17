#include <stdlib.h>
#include "sprite.h"


/*
void scene_init(struct Game *game);

void scene_change(struct Game *game, int oldSc, int newSc);

void scene_exit(struct Game *game);
*/

Sprite **init_sprite(short yLength, short xLength) {
	Sprite **spriteArray = malloc(sizeof(Sprite *) * yLength); // the sprite
	for (int y = 0; y < yLength; y++) {
		spriteArray[y] = malloc(sizeof(Sprite) * xLength);
		for (int x = 0; x < xLength; x++) {
			Sprite *sprite = &(spriteArray[y][x]);
			sprite->colour = NULL;
			sprite->icon = NULL;
			sprite->button = NULL;

			sprite->colourDepth = 0;
			sprite->iconDepth = 0;
			sprite->buttonDepth = 0;
		}
	}
	return spriteArray;
}

void free_sprite(short yLength, short xLength, Sprite **sprite) {
	for (int y = 0; y < yLength; y++) {
		for (int x = 0; x < xLength; x++) {
			for (int i = 0; i < sprite[y][x].iconDepth; i++) {
				free(sprite[y][x].icon[i]);
			}
			if (sprite[y][x].icon != NULL) {
				free(sprite[y][x].icon);
			}
			if (sprite[y][x].colour != NULL) {
				free(sprite[y][x].colour);
			}
			if (sprite[y][x].button != NULL) {
				free(sprite[y][x].button);
			}
		}
		free(sprite[y]);
	}
	free(sprite);
	return;
}
