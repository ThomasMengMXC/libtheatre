#include <curses.h>
#include <string.h>

#include "layer.h"

static int add_x_to_y(uint16_t *depth, uint16_t *maxDepth,
		void **object, size_t size);
static int remove_x_from_y(uint16_t *depth, uint16_t *maxDepth,
		void **object, size_t size);


Layer *init_layer(char fillSize, short yOffset, short xOffset,
		uint16_t yLength, uint16_t xLength) {
	Layer *layer = malloc(sizeof(Layer));
	layer->visibility = 1;
	layer->fillSize = fillSize;
	layer->yOffset = yOffset; layer->xOffset = xOffset;
	layer->yLength = yLength; layer->xLength = xLength;
	layer->sprite = init_sprite(yLength, xLength);
	layer->update = NULL;
	return layer;
}

void resize_layer(Layer *layer, uint16_t yLength, uint16_t xLength) {
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
	int y, x;
	if (!layer) return;
	for (y = 0; y < layer->yLength; y++) {
		for (x = 0; x < layer->xLength; x++) {
			Vector2D pos;
			pos.y = y + layer->yOffset;
			pos.x = x + layer->xOffset;
			vector2D_push(layer->update, pos);
		}
	}
}

/* ADDING/REMOVING TO/FROM LAYERS ---------------------------------------*/

void add_colour_to_layer(Layer *layer, short y, short x, Colour colour) {
	Sprite *sprite;
	Vector2D pos;
	if (!layer) return;
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	sprite = &(layer->sprite[y][x]);
	if (add_x_to_y(&(sprite->colourDepth), &(sprite->colourMaxDepth),
			(void **) &(sprite->colour), sizeof(Colour))) {
		return;
	}
	pos.y = y + layer->yOffset;
	pos.x = x + layer->xOffset;
	vector2D_push(layer->update, pos);
	sprite->colour[sprite->colourDepth - 1] = colour;
}

void remove_colour_from_layer(Layer *layer, short y, short x) {
	Sprite *sprite;
	Vector2D pos;
	if (!layer) return;
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) return;
	sprite = &(layer->sprite[y][x]);
	if (remove_x_from_y(&(sprite->colourDepth), &(sprite->colourMaxDepth),
			(void **) &(sprite->colour), sizeof(Colour))) {
		pos.y = y + layer->yOffset;
		pos.x = x + layer->xOffset;
		vector2D_push(layer->update, pos);
	}
}

void add_icon_to_layer(Layer *layer, short y, short x, char *icon, size_t n) {
	int i, cells;
	Vector2D pos;
	Sprite *sprite;
	if (!layer || 
			y < 0 || y >= layer->yLength || 
			x < 0 || x >= layer->xLength) {
		return;
	}

	cells = (n % 2 ? n + 1 : n) >> 1; /* calculate the cel of n/2 */
	for (i = 0; i < cells; i++, x++) {
		/* return if x pos is beyond the size of the layer */
		if (x >= layer->xLength) return;
		sprite = &(layer->sprite[y][x]);

		/* Add a new char[3] block of memory to the sprite */
		add_x_to_y(&(sprite->iconDepth), &(sprite->iconMaxDepth),
				(void **) &(sprite->icon), sizeof(char[3]));

		/* push the position onto the update struct */
		pos.y = y + layer->yOffset;
		pos.x = x + layer->xOffset;
		vector2D_push(layer->update, pos);

		if (i + 1 == cells && n % 2) {
			/* copy an empty character if the length of the string is 1 */
			strncpy(sprite->icon[sprite->iconDepth - 1], icon + i * 2, 1);
			sprite->icon[sprite->iconDepth - 1][1] = ' ';
		} else {
			/* copy as normal */
			strncpy(sprite->icon[sprite->iconDepth - 1], icon + i * 2, 2);
		}

		/* null terminator */
		sprite->icon[sprite->iconDepth - 1][2] = 0;
	}
}

void remove_icon_from_layer(Layer *layer, short y, short x, size_t n) {
	int i;
	Sprite *sprite;
	Vector2D pos;
	if (!layer ||
			y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	n = n % 2 ? (n + 1) >> 1 : n >> 1; /* ceil of n/2 */
	for (i = 0; i < n; i++) {
		if (x >= layer->xLength) return;
		sprite = &(layer->sprite[y][x]);
		if (remove_x_from_y(&(sprite->iconDepth), &(sprite->iconMaxDepth),
				(void **) &(sprite->icon), sizeof(char[3]))) {
			pos.y = y + layer->yOffset;
			pos.x = x + layer->xOffset;
			vector2D_push(layer->update, pos);
		}
		x += 1;
	}
}

void add_button_to_layer(Layer *layer, short y, short x, Button button) {
	Sprite *sprite;
	if (!layer ||
			y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	sprite = &(layer->sprite[y][x]);
	add_x_to_y(&(sprite->buttonDepth), &(sprite->buttonMaxDepth),
			(void **) &(sprite->button), sizeof(Button));
	sprite->button[sprite->buttonDepth - 1] = button;
}

void remove_button_from_layer(Layer *layer, short y, short x) {
	Sprite *sprite;
	if (!layer ||
			y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	sprite = &(layer->sprite[y][x]);
	remove_x_from_y(&(sprite->buttonDepth), &(sprite->buttonMaxDepth),
			(void **) &(sprite->button), sizeof(Button));
}

void add_hover_to_layer(Layer *layer, short y, short x, Hover hover) {
	Sprite *sprite;
	if (!layer ||
			y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	sprite = &(layer->sprite[y][x]);
	add_x_to_y(&(sprite->hoverDepth), &(sprite->hoverMaxDepth),
			(void **) &(sprite->hover), sizeof(Hover));
	sprite->hover[sprite->hoverDepth - 1] = hover;
}

void remove_hover_from_layer(Layer *layer, short y, short x) {
	Sprite *sprite;
	if (!layer ||
			y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	sprite = &(layer->sprite[y][x]);
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

/* STATIC -----------------------------------------------------------------*/

static int add_x_to_y(uint16_t *depth, uint16_t *maxDepth,
		void **object, size_t size) {
	if (*depth >= (UINT16_MAX >> 1) - 1) return 1;
	(*depth)++;
	if (*depth >= *maxDepth) {
		*maxDepth = *depth * 2;
		*object = realloc(*object, size * *maxDepth);
	}
	return 0;
}

static int remove_x_from_y(uint16_t *depth, uint16_t *maxDepth,
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
