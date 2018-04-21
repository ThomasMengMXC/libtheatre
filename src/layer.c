#include <curses.h>
#include <stdlib.h>
#include <string.h>

#include "layer.h"

static int position_test(Layer **layer, short y, short x, uint8_t depth);
static int add_x_to_y(uint8_t *depth, uint8_t *maxDepth,
		void **object, size_t size);
static int remove_x_from_y(uint8_t *depth, uint8_t *maxDepth,
		void **object, size_t size);


Layer *init_layer(short yOffset, short xOffset, short yLength, short xLength) {
	Layer *layer = malloc(sizeof(Layer));
	layer->visibility = 1;
	layer->yOffset = yOffset; layer->xOffset = xOffset;
	layer->yLength = yLength; layer->xLength = xLength;
	layer->sprite = init_sprite(yLength, xLength);
	layer->update = NULL;
	return layer;
}

void clear_layer(Layer *layer) {
	free_sprite(layer->sprite, layer->yLength, layer->xLength);
	refresh_layer(layer);
	layer->sprite = init_sprite(layer->yLength, layer->xLength);
	return;
}

void mv_layer_relative(Layer *layer, short y, short x) {
	refresh_layer(layer);
	layer->yOffset += y;
	layer->xOffset += x;
	refresh_layer(layer);
	return;
}

void mv_layer_absolute(Layer *layer, short y, short x) {
	refresh_layer(layer);
	layer->yOffset = y;
	layer->xOffset = x;
	refresh_layer(layer);
	return;
}

void free_layer(Layer *layer) {
	free_sprite(layer->sprite, layer->yLength, layer->xLength);
	refresh_layer(layer);
	free(layer);
	return;
}

/*
// Returns 1 if nothing is drawn, 0 otherwise
int paint_colour(Layer **layer, short y, short x, uint8_t colourLayer) {
	int result = position_test(layer, y, x, colourLayer);
	if (result >= 0) return result;

	Layer *lyr = layer[colourLayer - 1];

	Sprite *sprite = lyr->sprite[y - lyr->yOffset] + x - lyr->xOffset;
	if (sprite->colourDepth == 0) return 1; // continue searching

	attron(COLOR_PAIR(mix_colours(sprite->colour, sprite->colourDepth) + 1));

	return 0; //stop
}
*/

// Returns 1 if nothing is drawn, 0 otherwise
int draw_icon(Layer **layer,short y, short x, uint8_t iconLayer) {
	int result = position_test(layer, y, x, iconLayer);
	if (result >= 0) return result; // either continue or stop

	Layer *lyr = layer[iconLayer - 1];

	Sprite *sprite = lyr->sprite[y - lyr->yOffset] + x - lyr->xOffset;
	if (sprite->iconDepth == 0) return 1; // continue

	mvprintw(y, 2 * x, sprite->icon[sprite->iconDepth - 1]);
	return 0;
}

void refresh_layer(Layer *layer) {
	for (int y = 0; y < layer->yLength; y++) {
		for (int x = 0; x < layer->xLength; x++) {
			Vector2D pos = {
				.y = y + layer->yOffset,
				.x = x + layer->xOffset
			};
			vector2D_push(layer->update, pos);
		}
	}
}

// ADDING/REMOVING TO/FROM LAYERS ----------------------------------------

void add_colour_to_layer(Layer *layer, short y, short x, Colour colour) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	add_x_to_y(&(sprite->colourDepth), &(sprite->colourMaxDepth),
			(void **) &(sprite->colour), sizeof(Colour));
	Vector2D pos = {.y = y + layer->yOffset, .x = x + layer->xOffset};
	vector2D_push(layer->update, pos);
	sprite->colour[sprite->colourDepth - 1] = colour;
	return;
}

void remove_colour_from_layer(Layer *layer, short y, short x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) return;
	Sprite *sprite = &(layer->sprite[y][x]);
	if (remove_x_from_y(&(sprite->colourDepth), &(sprite->colourMaxDepth),
			(void **) &(sprite->colour), sizeof(Colour))) {
		Vector2D pos = {.y = y + layer->yOffset, .x = x + layer->xOffset};
		vector2D_push(layer->update, pos);
	}
}

void add_icon_to_layer(Layer *layer, short y, short x, char *icon, size_t n) {
	n = n % 2 ? (n + 1) >> 1 : n >> 1; // ceil of n/2
	for (int i = 0; i < n; i++) {
		if (	y < 0 || y >= layer->yLength ||
				x < 0 || x >= layer->xLength) {
			return;
		}
		Sprite *sprite = &(layer->sprite[y][x]);
		add_x_to_y(&(sprite->iconDepth), &(sprite->iconMaxDepth),
				(void **) &(sprite->icon), sizeof(char[3]));
		Vector2D pos = {.y = y + layer->yOffset, .x = x + layer->xOffset};
		vector2D_push(layer->update, pos);
		strcpy(sprite->icon[sprite->iconDepth - 1], icon + i * 2);
		x += 1;
	}
}

void remove_icon_from_layer(Layer *layer, short y, short x, size_t n) {
	n = n % 2 ? (n + 1) >> 1 : n >> 1; // ceil of n/2
	for (int i = 0; i < n; i++) {
		if (	y < 0 || y >= layer->yLength ||
				x < 0 || x >= layer->xLength) return;
		Sprite *sprite = &(layer->sprite[y][x]);
		if (remove_x_from_y(&(sprite->iconDepth), &(sprite->iconMaxDepth),
				(void **) &(sprite->icon), sizeof(char[3]))) {
			Vector2D pos = {.y = y + layer->yOffset, .x = x + layer->xOffset};
			vector2D_push(layer->update, pos);
		}
		x += 1;
	}
}

void add_button_to_layer(Layer *layer, short y, short x, Button button) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	add_x_to_y(&(sprite->buttonDepth), &(sprite->buttonMaxDepth),
			(void **) &(sprite->button), sizeof(Button));
	sprite->button[sprite->buttonDepth - 1] = button;
	return;
}

void remove_button_from_layer(Layer *layer, short y, short x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) return;
	Sprite *sprite = &(layer->sprite[y][x]);
	remove_x_from_y(&(sprite->buttonDepth), &(sprite->buttonMaxDepth),
			(void **) &(sprite->button), sizeof(Button));
}

void add_hover_to_layer(Layer *layer, short y, short x, Hover hover) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	add_x_to_y(&(sprite->hoverDepth), &(sprite->hoverMaxDepth),
			(void **) &(sprite->hover), sizeof(Hover));
	sprite->hover[sprite->hoverDepth - 1] = hover;
	return;
}

void remove_hover_from_layer(Layer *layer, short y, short x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) return;
	Sprite *sprite = &(layer->sprite[y][x]);
	remove_x_from_y(&(sprite->hoverDepth), &(sprite->hoverMaxDepth),
			(void **) &(sprite->hover), sizeof(Hover));
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

// STATIC -------------------------------------------------------------------

static int position_test(Layer **layer, short y, short x, uint8_t depth) {
	if (depth < 1) return 0;
	Layer *lyr = layer[depth - 1];
	y -= lyr->yOffset;
	x -= lyr->xOffset;
	if (lyr->visibility == 0 ||
			y < 0 || y >= lyr->yLength ||
			x < 0 || x >= lyr->xLength) {
		return 1;
	}
	return -1;
}

static int add_x_to_y(uint8_t *depth, uint8_t *maxDepth,
		void **object, size_t size) {
	(*depth)++;
	if (*depth >= *maxDepth) {
		*maxDepth = *depth * 2;
		*object = realloc(*object, size * *maxDepth);
	}
	return 0;
}

static int remove_x_from_y(uint8_t *depth, uint8_t *maxDepth,
		void **object, size_t size) {
	if (*depth) {
		(*depth)--;
		while (*depth * 2 < *maxDepth) {
			*maxDepth = *depth * 3 / 2;
			if (*maxDepth == 0) {
				free(*object);
				*object = NULL;
			} else {
				*object = realloc(*object, size * *maxDepth);
			}
		}
		return 1;
	}
	return 0;
}
