#include <curses.h>
#include <stdlib.h>
#include <string.h>

#include "layer.h"

static int position_test(Layer **layer, short y, short x, char depth);
static int add_x_to_y(char *depth, char *maxDepth,
		void **object, size_t size);
static int remove_x_from_y(char *depth, char *maxDepth,
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

// Returns 1 if nothing is drawn, 0 otherwise
int paint_colour(Layer **layer, short y, short x, char colourLayer) {
	int result = position_test(layer, y, x, colourLayer);
	if (result >= 0) return result;

	Layer *lyr = layer[colourLayer - 1];

	Sprite *sprite = lyr->sprite[y - lyr->yOffset] + x - lyr->xOffset;
	if (sprite->colourDepth == 0) return 1; // continue searching

	attron(COLOR_PAIR(sprite->colour[sprite->colourDepth - 1] + 1));

	return 0; //stop
}

// Returns 1 if nothing is drawn, 0 otherwise
int draw_icon(Layer **layer,short y, short x, char iconLayer) {
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
			Vector2D position = {
				.y = y + layer->yOffset,
				.x = x + layer->xOffset
			};
			vector2D_push(layer->update, position);
		}
	}
}

// ADDING/REMOVING TO/FROM LAYERS ----------------------------------------

void add_colour_to_layer(Layer *layer, short y, short x, uint8_t colour) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	add_x_to_y(&(sprite->colourDepth), &(sprite->colourMaxDepth),
			(void **) &(sprite->colour), sizeof(uint8_t));
	Vector2D position = {
		.y = y + layer->yOffset,
		.x = x + layer->xOffset
	};
	vector2D_push(layer->update, position);
	sprite->colour[sprite->colourDepth - 1] = colour;
	return;
}

void remove_colour_from_layer(Layer *layer, short y, short x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) return;
	Sprite *sprite = &(layer->sprite[y][x]);
	if (remove_x_from_y(&(sprite->colourDepth), &(sprite->colourMaxDepth),
			(void **) &(sprite->colour), sizeof(uint8_t))) {
		Vector2D position = {
			.y = y + layer->yOffset,
			.x = x + layer->xOffset
		};
		vector2D_push(layer->update, position);
	}
}

void add_icon_to_layer(Layer *layer, short y, short x, char *icon) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	add_x_to_y(&(sprite->iconDepth), &(sprite->iconMaxDepth),
			(void **) &(sprite->icon), sizeof(char[3]));
	Vector2D position = {
		.y = y + layer->yOffset,
		.x = x + layer->xOffset
	};
	vector2D_push(layer->update, position);
	strcpy(sprite->icon[sprite->iconDepth - 1], icon);
	return;
}

void remove_icon_from_layer(Layer *layer, short y, short x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) return;
	Sprite *sprite = &(layer->sprite[y][x]);
	if (remove_x_from_y(&(sprite->iconDepth), &(sprite->iconMaxDepth),
			(void **) &(sprite->icon), sizeof(char[3]))) {
		Vector2D position = {
			.y = y + layer->yOffset,
			.x = x + layer->xOffset
		};
		vector2D_push(layer->update, position);
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

static int position_test(Layer **layer, short y, short x, char depth) {
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

static int add_x_to_y(char *depth, char *maxDepth,
		void **object, size_t size) {
	(*depth)++;
	if (*depth >= *maxDepth) {
		*maxDepth = *depth * 2;
		*object = realloc(*object, size * *maxDepth);
	}
	return 0;
}

static int remove_x_from_y(char *depth, char *maxDepth,
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
