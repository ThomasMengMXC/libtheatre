#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "layer.h"
#include "sprite.h"

Layer *init_layer(short yOffset, short xOffset, short yLength, short xLength) {
	Layer *layer = malloc(sizeof(Layer));
	layer->visibility = 1;
	layer->yOffset = yOffset; layer->xOffset = xOffset;
	layer->yLength = yLength; layer->xLength = xLength;
	layer->sprite = init_sprite(yLength, xLength);
	layer->update = NULL;
	return layer;
}

void free_layer(Layer *layer) {
	free_sprite(layer->yLength, layer->xLength, layer->sprite);
	free(layer);
	return;
}

// Returns 1 if nothing is drawn, 0 otherwise
int activate_colour(Layer **layer, short y, short x, char colourLayer) {
	int result = position_test(layer, y, x, colourLayer);
	if (result >= 0) return result;

	Layer *lyr = layer[colourLayer - 1];
	if ((*lyr->update)[y][x] == 0) return 1;

	Sprite *sprite = lyr->sprite[y - lyr->yOffset] + x - lyr->xOffset;
	if (sprite->colourDepth == 0) return 1;

	RGB col = sprite->colour[sprite->colourDepth - 1];
	int termColour = 0;
	if (col.term == 256)	termColour = rgb_to_term256(col.r, col.g, col.b);
	else if (col.term == 16)termColour = rgb_to_term16(col.r, col.g, col.b);
	else 					termColour = rgb_to_term8(col.r, col.g, col.b);
	attron(COLOR_PAIR(termColour));

	return 0;
}

int deactivate_colour(Layer **layer, short y, short x, char colourLayer) {
	int result = position_test(layer, y, x, colourLayer);
	if (result >= 0) return result;

	Layer *lyr = layer[colourLayer - 1];
	if ((*lyr->update)[y][x] == 0) return 0;

	Sprite *sprite = lyr->sprite[y - lyr->yOffset] + x - lyr->xOffset;

	RGB col = sprite->colour[sprite->colourDepth - 1];
	int termColour = 0;
	if (col.term == 255)	termColour = rgb_to_term256(col.r, col.g, col.b);
	else if (col.term == 16)termColour = rgb_to_term16(col.r, col.g, col.b);
	else					termColour = rgb_to_term8(col.r, col.g, col.b);
	attroff(COLOR_PAIR(termColour));

	(*lyr->update)[y][x] = 0;
	return 0;
}

// Returns 1 if nothing is drawn, 0 otherwise
int draw_icon(Layer **layer,short y, short x, char iconLayer) {
	int result = position_test(layer, y, x, iconLayer);
	if (result >= 0) return result; // either continue or stop

	Layer *lyr = layer[iconLayer - 1];
	if ((*lyr->update)[y][x] == 0) return 0; //stop

	Sprite *sprite = lyr->sprite[y - lyr->yOffset] + x - lyr->xOffset;
	if (sprite->iconDepth == 0) return 1; // continue

	mvprintw(y, 2 * x, sprite->icon[sprite->iconDepth - 1]);

	(*lyr->update)[y][x] = 0;
	return 0;
}

int position_test(Layer **layer, short y, short x, char depth) {
	if (depth < 1) return 0;

	Layer *lyr = layer[depth - 1];
	short yRelative = y - lyr->yOffset;
	short xRelative = x - lyr->xOffset;
	if (lyr->visibility == 0 ||
			yRelative < 0 || yRelative >= lyr->yLength ||
			xRelative < 0 || xRelative >= lyr->xLength) {
		return 1;
	}
	return -1;
}

void add_colour_to_layer(Layer *layer, short y, short x, short term, 
		uint8_t r, uint8_t g, uint8_t b) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	(*layer->update)[y + layer->yOffset][x + layer->xOffset] = 1;

	Sprite *sprite = &(layer->sprite[y][x]);
	sprite->colourDepth++;
	if (sprite->colourDepth >= sprite->colourMaxDepth) {
		sprite->colourMaxDepth = sprite->colourDepth * 2;
		sprite->colour = realloc(sprite->colour,
				sizeof(RGB) * sprite->colourMaxDepth);
	}
	RGB col = {
		.term = term,
		.r = r, .g = g, .b = b
	};
	sprite->colour[sprite->colourDepth - 1] = col;
	return;
}

void remove_colour_from_layer(Layer *layer, short y, short x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);

	if (sprite->colourDepth) {
		(*layer->update)[y + layer->yOffset][x + layer->xOffset] = 1;
		sprite->colourDepth--;
		while (sprite->colourDepth * 2 < sprite->colourMaxDepth) {
			sprite->colourMaxDepth = sprite->colourDepth * 3 / 2;
			if (sprite->colourMaxDepth == 0) {
				free(sprite->colour);
				sprite->colour = NULL;
			} else {
				sprite->colour = realloc(sprite->colour,
						sizeof(RGB) * sprite->colourMaxDepth);
			}
		}
	}
	return;
}

void add_icon_to_layer(Layer *layer, short y, short x, char *icon) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	(*layer->update)[y + layer->yOffset][x + layer->xOffset] = 1;

	Sprite *sprite = &(layer->sprite[y][x]);
	sprite->iconDepth++;
	if (sprite->iconDepth >= sprite->iconMaxDepth) {
		sprite->iconMaxDepth = sprite->iconDepth * 2;
		sprite->icon = realloc(sprite->icon,
				sizeof(char *) * sprite->iconMaxDepth);
	}
	sprite->icon[sprite->iconDepth - 1]
		= malloc(sizeof(char) * strlen(icon) + 1);
	strcpy(sprite->icon[sprite->iconDepth - 1], icon);
	return;
}

void remove_icon_from_layer(Layer *layer, short y, short x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	if (sprite->iconDepth) {
		(*layer->update)[y + layer->yOffset][x + layer->xOffset] = 1;

		free(sprite->icon[sprite->iconDepth - 1]);
		sprite->icon[sprite->iconDepth - 1] = NULL;

		sprite->iconDepth--;
		while (sprite->iconDepth * 2 < sprite->iconMaxDepth) {
			sprite->iconMaxDepth = sprite->iconDepth * 3 / 2;
			if (sprite->iconMaxDepth == 0) {
				free(sprite->icon);
				sprite->icon = NULL;
			} else {
				sprite->icon = realloc(sprite->icon,
						sizeof(char *) * sprite->iconMaxDepth);
			}
		}
	}
	return;
}

void add_button_to_layer(Layer *layer, short y, short x, Button button) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	sprite->buttonDepth++;
	if (sprite->buttonDepth >= sprite->buttonMaxDepth) {
		sprite->buttonMaxDepth = sprite->buttonDepth * 2;
		sprite->button = realloc(sprite->button,
				sizeof(Button) * sprite->buttonMaxDepth);
	}
	sprite->button[sprite->buttonDepth - 1] = button;
	return;
}

void remove_button_from_layer(Layer *layer, short y, short x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);

	if (sprite->buttonDepth) {
		(*layer->update)[y + layer->yOffset][x + layer->xOffset] = 1;
		sprite->buttonDepth--;
		while (sprite->buttonDepth * 2 < sprite->buttonMaxDepth) {
			sprite->buttonMaxDepth = sprite->buttonDepth * 3 / 2;
			if (sprite->buttonMaxDepth == 0) {
				free(sprite->button);
				sprite->button = NULL;
			} else {
				sprite->button = realloc(sprite->button,
						sizeof(Button) * sprite->buttonMaxDepth);
			}
		}
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

