#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "layer.h"

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
	refresh_layer(layer);
	free_sprite(layer->sprite, layer->yLength, layer->xLength);
	layer->yLength = yLength;
	layer->xLength = xLength;
	layer->sprite = init_sprite(yLength, xLength);
}
	

void clear_layer(Layer *layer) {
	free_sprite(layer->sprite, layer->yLength, layer->xLength);
	refresh_layer(layer);
	layer->sprite = init_sprite(layer->yLength, layer->xLength);
}

void mv_layer_relative(Layer *layer, unsigned y, unsigned x) {
	refresh_layer(layer);
	layer->yOffset += y;
	layer->xOffset += x;
	refresh_layer(layer);
}

void mv_layer_absolute(Layer *layer, unsigned y, unsigned x) {
	refresh_layer(layer);
	layer->yOffset = y;
	layer->xOffset = x;
	refresh_layer(layer);
}

void free_layer(Layer *layer) {
	free_sprite(layer->sprite, layer->yLength, layer->xLength);
	refresh_layer(layer);
	free(layer);
}

void refresh_layer(Layer *layer) {
	for (unsigned y = 0; y < layer->yLength; y++) {
		for (unsigned x = 0; x < layer->xLength; x++) {
			Vector2D pos;
			pos.y = y + layer->yOffset;
			pos.x = x + layer->xOffset;
			vector2D_push(layer->update, pos);
		}
	}
}

// ADDING/REMOVING TO/FROM LAYERS ----------------------------------------

void add_colour_to_layer(Layer *layer, unsigned y, unsigned x, Colour colour) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	uVec_push(sprite->colour, &colour);
	Vector2D pos;
	pos.y = y + layer->yOffset;
	pos.x = x + layer->xOffset;
	vector2D_push(layer->update, pos);
}

void remove_colour_from_layer(Layer *layer, unsigned y, unsigned x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength)
		return;
	Sprite *sprite = &(layer->sprite[y][x]);
	if (sprite->colour->length != 0) {
		uVec_pop(sprite->colour);
		Vector2D pos;
		pos.y = y + layer->yOffset;
		pos.x = x + layer->xOffset;
		vector2D_push(layer->update, pos);
	}
}

void add_icon_to_layer(Layer *layer, unsigned y, unsigned x,
		const char *icon, size_t n) {
	unsigned cells = (n % 2 ? n + 1 : n) >> 1;
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength)
		return;
	for (unsigned i = 0; i < cells; i++, x++) {
		char subicon[3];
		if (x >= layer->xLength)
			return;
		if (i + 1 == cells && n % 2) {
			subicon[0] = (icon + i * 2)[0];
			subicon[1] = ' ';
		} else {
			subicon[0] = (icon + i * 2)[0];
			subicon[1] = (icon + i * 2)[1];
		}
		subicon[2] = '\0';
		Sprite *sprite = &(layer->sprite[y][x]);
		uVec_push(sprite->icon, subicon);
		Vector2D pos;
		pos.y = y + layer->yOffset;
		pos.x = x + layer->xOffset;
		vector2D_push(layer->update, pos);
	}
}

void remove_icon_from_layer(Layer *layer, unsigned y, unsigned x, size_t n) {
	n = n % 2 ? (n + 1) >> 1 : n >> 1; // ceil of n/2
	while (x < n) {
		if (	y < 0 || y >= layer->yLength ||
				x < 0 || x >= layer->xLength)
			return;
		Sprite *sprite = &(layer->sprite[y][x]);
		if (sprite->icon->length != 0) {
			uVec_pop(sprite->icon);
			Vector2D pos;
			pos.y = y + layer->yOffset;
			pos.x = x + layer->xOffset;
			vector2D_push(layer->update, pos);
		}
		x++;
	}
}

void add_button_to_layer(Layer *layer, unsigned y, unsigned x, Button button) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	uVec_push(sprite->button, &button);
}

void remove_button_from_layer(Layer *layer, unsigned y, unsigned x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength)
		return;
	Sprite *sprite = &(layer->sprite[y][x]);
	uVec_pop(sprite->button);
}

void add_hover_to_layer(Layer *layer, unsigned y, unsigned x, Hover hover) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	uVec_push(sprite->hover, &hover);
}

void remove_hover_from_layer(Layer *layer, unsigned y, unsigned x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) return;
	Sprite *sprite = &(layer->sprite[y][x]);
	uVec_pop(sprite->hover);
}

void add_attr_to_layer(Layer *layer, unsigned y, unsigned x, attr_t attr) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	uVec_push(sprite->attr, &attr);
}

void remove_attr_from_layer(Layer *layer, unsigned y, unsigned x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength)
		return;
	Sprite *sprite = &(layer->sprite[y][x]);
	uVec_pop(sprite->attr);
}

void append_attr_to_layer(Layer *layer, unsigned y, unsigned x, attr_t attr) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	if (sprite->attr->length != 0) {
		attr_t tmp = (*(attr_t *) uVec_get_back(sprite->attr)) & ~attr;
		uVec_replace(sprite->attr, sprite->attr->length - 1, &tmp);
	}
}

void truncate_attr_from_layer(Layer *layer, unsigned y, unsigned x,
		attr_t attr) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength)
		return;
	Sprite *sprite = &(layer->sprite[y][x]);
	if (sprite->attr->length != 0) {
		attr_t tmp = (*(attr_t *) uVec_get_back(sprite->attr)) & ~attr;
		uVec_replace(sprite->attr, sprite->attr->length - 1, &tmp);
	}
}

void layer_swap(Layer **layer1, Layer **layer2) {
	void *swap;
	swap = *layer1;
	*layer1 = *layer2;
	*layer2 = (Layer *) swap;
}

void layer_memory_swap(Layer *layer1, Layer *layer2) {
	Layer swapLayer;
	memcpy((void *) &swapLayer, (void *) layer1, sizeof(Layer));
	memcpy((void *) layer1, (void *) layer2, sizeof(Layer));
	memcpy((void *) layer2, (void *) &swapLayer, sizeof(Layer));
}
