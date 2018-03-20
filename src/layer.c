#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "layer.h"
#include "sprite.h"
#include "colour.h"

Layer *init_layer(short yOffset, short xOffset, short yLength, short xLength) {
	Layer *layer = malloc(sizeof(Layer));
	layer->draw = true;
	layer->visibility = true; // setting variables
	layer->yOffset = yOffset; layer->xOffset = xOffset;
	layer->yLength = yLength; layer->xLength = xLength;
	layer->sprite = init_sprite(yLength, xLength);
	return layer;
}

void free_layer(Layer *layer) {
	free_sprite(layer->yLength, layer->xLength, layer->sprite);
	free(layer);
	return;
}

// Returns 1 if nothing is drawn, 0 otherwise
int activate_colour(Layer **layer, short y, short x, char colourLayer) {
	Layer *lyr = layer[colourLayer - 1];

	int result = position_test(layer, y, x, colourLayer);
	if (result >= 0) return result;

	Sprite *sprite = lyr->sprite[y - lyr->yOffset] + x - lyr->xOffset;
	if (sprite->colourDepth == 0) return 1;

	RGB col = sprite->colour[sprite->colourDepth - 1];
	int termColour = 0;
	if (col.term == 255) termColour = rgb_to_term256(col.r, col.g, col.b);
	else if (col.term == 16) termColour = rgb_to_term16(col.r, col.g, col.b);
	else termColour = rgb_to_term8(col.r, col.g, col.b);
	attron(COLOR_PAIR(termColour));

	lyr->draw = false;
	return 0;
}

int deactivate_colour(Layer **layer, short y, short x, char colourLayer) {
	Layer *lyr = layer[colourLayer - 1];

	int result = position_test(layer, y, x, colourLayer);
	if (result >= 0) return result;

	Sprite *sprite = lyr->sprite[y - lyr->yOffset] + x - lyr->xOffset;
	if (sprite->colourDepth == 0) return 1;

	RGB col = sprite->colour[sprite->colourDepth - 1];
	int termColour = 0;
	if (col.term == 255) termColour = rgb_to_term256(col.r, col.g, col.b);
	if (col.term == 16) termColour = rgb_to_term16(col.r, col.g, col.b);
	else termColour = rgb_to_term8(col.r, col.g, col.b);

	attroff(COLOR_PAIR(termColour));
	return 0;
}

// Returns 1 if nothing is drawn, 0 otherwise
int draw_icon(Layer **layer,short y, short x, char iconLayer) {
	Layer *lyr = layer[iconLayer - 1];

	int result = position_test(layer, y, x, iconLayer);
	if (result >= 0) return result;

	Sprite *sprite = lyr->sprite[y - lyr->yOffset] + x - lyr->xOffset;
	if (sprite->iconDepth == 0) return 1;

	mvprintw(y, 2 * x, sprite->icon[sprite->iconDepth - 1]);
	lyr->draw = false;
	return 0;
}

int position_test(Layer **layer, short y, short x, char depth) {
	if (depth < 1) return 0;

	Layer *lyr = layer[depth - 1];
	short yRelative = y - lyr->yOffset;
	short xRelative = x - lyr->xOffset;
	if (lyr->visibility == false ||
			yRelative < 0 || yRelative >= lyr->yLength ||
			xRelative < 0 || xRelative >= lyr->xLength) {
		return 1;
	}
	return -1;
}

void add_colour_to_layer(Layer *layer, short y, short x, uint8_t term, 
		uint8_t r, uint8_t g, uint8_t b) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	sprite->colourDepth++;
	sprite->colour = realloc(sprite->colour,
			sizeof(RGB) * sprite->colourDepth);
	RGB col = {
		.term = term,
		.r = r, .g = g, .b = b
	};
	sprite->colour[sprite->colourDepth - 1] = col;
	layer->draw = true;
	return;
}

void remove_colour_from_layer(Layer *layer, short y, short x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	if (sprite->colourDepth < 1) return;

	if (sprite->colourDepth > 1) {
		sprite->colourDepth--;
		sprite->colour = realloc(sprite->colour,
				sizeof(RGB) * sprite->colourDepth);
	} else if (sprite->colourDepth == 1) {
		sprite->colourDepth--;
		free(sprite->colour);
		sprite->colour = NULL;
	}
	layer->draw = true;
	return;
}

void add_icon_to_layer(Layer *layer, short y, short x, char *icon) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	sprite->iconDepth++;
	sprite->icon = realloc(sprite->icon, sizeof(char *) * sprite->iconDepth);

	sprite->icon[sprite->iconDepth - 1] = malloc(sizeof(char) * 3);
	strncpy(sprite->icon[sprite->iconDepth - 1], icon, 3);
	layer->draw = true;
	return;
}

void remove_icon_from_layer(Layer *layer, short y, short x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	if (sprite->iconDepth < 1) return;

	free(sprite->icon[sprite->iconDepth - 1]);
	sprite->icon[sprite->iconDepth - 1] = NULL;
	if (sprite->iconDepth > 1) {
		sprite->iconDepth--;
		sprite->icon = realloc(sprite->icon,
				sizeof(char *) * sprite->iconDepth);
	} else if (sprite->iconDepth == 1) {
		sprite->iconDepth--;
		free(sprite->icon);
		sprite->icon = NULL;
	}
	layer->draw = true;
	return;
}

void add_button_to_layer(Layer *layer, short y, short x, Button button) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	sprite->buttonDepth++;
	sprite->button = realloc(sprite->button,
			sizeof(Button) * sprite->buttonDepth);
	sprite->button[sprite->buttonDepth - 1] = button;
	return;
}

void remove_button_from_layer(Layer *layer, short y, short x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	if (sprite->buttonDepth < 1) return;

	if (sprite->buttonDepth > 1) {
		sprite->buttonDepth--;
		sprite->button = realloc(sprite->button,
				sizeof(Button) * sprite->buttonDepth);
	} else if (sprite->buttonDepth == 1) {
		sprite->buttonDepth--;
		free(sprite->button);
		sprite->icon = NULL;
	}
	return;
}

void layer_swap(Layer **layer1, Layer **layer2) {
	void *swap;
	swap = *layer1;
	*layer1 = *layer2;
	*layer2 = swap;
	return;
}

void layer_memory_swap(Layer *layer1, Layer *layer2) {
	Layer *swapLayer = malloc(sizeof(Layer));
	memcpy((void *) swapLayer, (void *) layer1, sizeof(Layer));
	memcpy((void *) layer1, (void *) layer2, sizeof(Layer));
	memcpy((void *) layer2, (void *) swapLayer, sizeof(Layer));
	free(swapLayer);
	return;
}

