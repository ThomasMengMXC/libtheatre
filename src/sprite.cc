extern "C" {
#include <ncurses.h>
}
#include <vector>
#include <array>
#include "sprite.h"

extern "C" {
Sprite **init_sprite(short yLength, short xLength) {
	Sprite **spriteArray = new Sprite*[yLength]();
	for (int y = 0; y < yLength; y++) {
		spriteArray[y] = new Sprite[xLength]();
		for (int x = 0; x < xLength; x++) {
			Sprite *sprite = &(spriteArray[y][x]);
			sprite->colour = (void *) new std::vector<Colour>();
			sprite->icon = (void *) new std::vector<std::array<char, 3>>();
			sprite->button = (void *) new std::vector<Button>();
			sprite->hover = (void *) new std::vector<Hover>();
			sprite->attr = (void *) new std::vector<attr_t>();
		}
	}
	return spriteArray;
}

void free_sprite(Sprite **sprite, short yLength, short xLength) {
	for (int y = 0; y < yLength; y++) {
		for (int x = 0; x < xLength; x++) {
			Sprite *tmp = &(sprite[y][x]);
			delete (std::vector<Colour> *) tmp->colour;
			delete (std::vector<std::array<char, 3>> *) tmp->icon;
			delete (std::vector<Button> *) tmp->button;
			delete (std::vector<Hover> *) tmp->hover;
			delete (std::vector<attr_t> *) tmp->attr;
		}
		delete[] sprite[y];
	}
	delete[] sprite;
}
}
