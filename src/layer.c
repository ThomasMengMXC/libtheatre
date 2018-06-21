#include <curses.h>
#include <string.h>

#include "layer.h"

static int add_x_to_y(unsigned *depth, unsigned *maxDepth,
		void **object, size_t size);
static int remove_x_from_y(unsigned *depth, unsigned *maxDepth,
		void **object, size_t size);


Layer *init_layer(char fillSize, short yOffset, short xOffset,
		unsigned yLength, unsigned xLength) {
	Layer *layer = malloc(sizeof(Layer));
	layer->visibility = 1;
	layer->fillSize = fillSize;
	layer->yOffset = yOffset; layer->xOffset = xOffset;
	layer->yLength = yLength; layer->xLength = xLength;
	layer->sprite = init_sprite(yLength, xLength);
	layer->update = NULL;
	return layer;
}

void resize_layer(Layer *layer, unsigned yLength, unsigned xLength) {
	if (!layer) return;
	refresh_layer(layer);
	free_sprite(layer->sprite, layer->yLength, layer->xLength);
	layer->yLength = yLength;
	layer->xLength = xLength;
	layer->sprite = init_sprite(yLength, xLength);
}
	

void clear_layer(Layer *layer) {
	if (!layer) return;
	free_sprite(layer->sprite, layer->yLength, layer->xLength);
	refresh_layer(layer);
	layer->sprite = init_sprite(layer->yLength, layer->xLength);
}

void mv_layer_relative(Layer *layer, short y, short x) {
	if (!layer) return;
	refresh_layer(layer);
	layer->yOffset += y;
	layer->xOffset += x;
	refresh_layer(layer);
}

void mv_layer_absolute(Layer *layer, short y, short x) {
	if (!layer) return;
	refresh_layer(layer);
	layer->yOffset = y;
	layer->xOffset = x;
	refresh_layer(layer);
}

void free_layer(Layer *layer) {
	if (!layer) return;
	free_sprite(layer->sprite, layer->yLength, layer->xLength);
	refresh_layer(layer);
	free(layer);
}

void refresh_layer(Layer *layer) {
	if (!layer) return;
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
	if (!layer) return;
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	if (add_x_to_y(&(sprite->colourDepth), &(sprite->colourMaxDepth),
			(void **) &(sprite->colour), sizeof(Colour))) {
		return;
	}
	Vector2D pos = {.y = y + layer->yOffset, .x = x + layer->xOffset};
	vector2D_push(layer->update, pos);
	sprite->colour[sprite->colourDepth - 1] = colour;
}

void remove_colour_from_layer(Layer *layer, short y, short x) {
	if (!layer) return;
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
	if (!layer) return;
	int cells = (n % 2 ? n + 1 : n) >> 1;
	if (y < 0 || y >= layer->yLength || x < 0 || x >= layer->xLength) return;
	for (int i = 0; i < cells; i++, x++) {
		if (x >= layer->xLength) return;
		Sprite *sprite = &(layer->sprite[y][x]);
		add_x_to_y(&(sprite->iconDepth), &(sprite->iconMaxDepth),
				(void **) &(sprite->icon), sizeof(char[3]));
		Vector2D pos = {.y = y + layer->yOffset, .x = x + layer->xOffset};
		vector2D_push(layer->update, pos);
		if (i + 1 == cells && n % 2) {
			strncpy(sprite->icon[sprite->iconDepth - 1], icon + i * 2, 1);
			sprite->icon[sprite->iconDepth - 1][1] = ' ';
		} else {
			strncpy(sprite->icon[sprite->iconDepth - 1], icon + i * 2, 2);
		}
		sprite->icon[sprite->iconDepth - 1][2] = 0;
	}
}

void remove_icon_from_layer(Layer *layer, short y, short x, size_t n) {
	if (!layer) return;
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
	if (!layer) return;
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	add_x_to_y(&(sprite->buttonDepth), &(sprite->buttonMaxDepth),
			(void **) &(sprite->button), sizeof(Button));
	sprite->button[sprite->buttonDepth - 1] = button;
}

void remove_button_from_layer(Layer *layer, short y, short x) {
	if (!layer) return;
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) return;
	Sprite *sprite = &(layer->sprite[y][x]);
	remove_x_from_y(&(sprite->buttonDepth), &(sprite->buttonMaxDepth),
			(void **) &(sprite->button), sizeof(Button));
}

void add_hover_to_layer(Layer *layer, short y, short x, Hover hover) {
	if (!layer) return;
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	add_x_to_y(&(sprite->hoverDepth), &(sprite->hoverMaxDepth),
			(void **) &(sprite->hover), sizeof(Hover));
	sprite->hover[sprite->hoverDepth - 1] = hover;
}

void remove_hover_from_layer(Layer *layer, short y, short x) {
	if (!layer) return;
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
}

void layer_memory_swap(Layer *layer1, Layer *layer2) {
	Layer *swapLayer = malloc(sizeof(Layer));
	memcpy((void *) swapLayer, (void *) layer1, sizeof(Layer));
	memcpy((void *) layer1, (void *) layer2, sizeof(Layer));
	memcpy((void *) layer2, (void *) swapLayer, sizeof(Layer));
	free(swapLayer);
}

// STATIC -------------------------------------------------------------------

static int add_x_to_y(unsigned *depth, unsigned *maxDepth,
		void **object, size_t size) {
	if (*depth >= (UINT16_MAX >> 1) - 1) return 1;
	(*depth)++;
	if (*depth >= *maxDepth) {
		*maxDepth = *depth * 2;
		*object = realloc(*object, size * *maxDepth);
	}
	return 0;
}

static int remove_x_from_y(unsigned *depth, unsigned *maxDepth,
		void **object, size_t size) {
	if (*depth) {
		(*depth)--;
		while (*depth * 4 < *maxDepth) {
			*maxDepth = *depth * 2;
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
