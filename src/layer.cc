#include <cstring>
#include <climits>
#include <utility>
#include <vector>
#include <array>

#include "layer.h"

extern "C" {
Layer *init_layer(char fillSize, short yOffset, short xOffset,
		unsigned yLength, unsigned xLength) {
	Layer *layer = new Layer();
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
	delete layer;
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
	auto vector = (std::vector<Colour> *) sprite->colour;
	vector->push_back(colour);
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
	auto vector = (std::vector<Colour> *) sprite->colour;
	if (!vector->empty()) {
		vector->pop_back();
		Vector2D pos;
		pos.y = y + layer->yOffset;
		pos.x = x + layer->xOffset;
		vector2D_push(layer->update, pos);
	}
}

void add_icon_to_layer(Layer *layer, unsigned y, unsigned x, char *icon, size_t n) {
	unsigned cells = (n % 2 ? n + 1 : n) >> 1;
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength)
		return;
	for (unsigned i = 0; i < cells; i++, x++) {
		if (x >= layer->xLength)
			return;
		Sprite *sprite = &(layer->sprite[y][x]);
		auto vector = (std::vector<std::array<char, 3>> *) sprite->icon;
		vector->push_back(std::array<char, 3>());
		Vector2D pos;
		pos.y = y + layer->yOffset;
		pos.x = x + layer->xOffset;
		vector2D_push(layer->update, pos);
		if (i + 1 == cells && n % 2) {
			vector->back()[0] = (icon + i * 2)[0];
			vector->back()[1] = ' ';
		} else {
			vector->back()[0] = (icon + i * 2)[0];
			vector->back()[1] = (icon + i * 2)[1];
		}
		vector->back()[2] = ' ';
	}
}

void remove_icon_from_layer(Layer *layer, unsigned y, unsigned x, size_t n) {
	n = n % 2 ? (n + 1) >> 1 : n >> 1; // ceil of n/2
	while (x < n) {
		if (	y < 0 || y >= layer->yLength ||
				x < 0 || x >= layer->xLength)
			return;
		Sprite *sprite = &(layer->sprite[y][x]);
		auto vector = (std::vector<std::array<char, 3>> *) sprite->icon;
		if (!vector->empty()) {
			vector->pop_back();
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
	auto vector = (std::vector<Button> *) sprite->button;
	vector->push_back(button);
}

void remove_button_from_layer(Layer *layer, unsigned y, unsigned x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength)
		return;
	Sprite *sprite = &(layer->sprite[y][x]);
	auto vector = (std::vector<Button> *) sprite->button;
	vector->pop_back();
}

void add_hover_to_layer(Layer *layer, unsigned y, unsigned x, Hover hover) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	auto vector = (std::vector<Hover> *) sprite->hover;
	vector->push_back(hover);
}

void remove_hover_from_layer(Layer *layer, unsigned y, unsigned x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) return;
	Sprite *sprite = &(layer->sprite[y][x]);
	auto vector = (std::vector<Hover> *) sprite->hover;
	vector->pop_back();
}

void add_attr_to_layer(Layer *layer, unsigned y, unsigned x, attr_t attr) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	auto vector = (std::vector<attr_t> *) sprite->attr;
	vector->push_back(attr);
}

void remove_attr_from_layer(Layer *layer, unsigned y, unsigned x) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength)
		return;
	Sprite *sprite = &(layer->sprite[y][x]);
	auto vector = (std::vector<attr_t> *) sprite->attr;
	vector->pop_back();
}

void append_attr_to_layer(Layer *layer, unsigned y, unsigned x, attr_t attr) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength) {
		return;
	}
	Sprite *sprite = &(layer->sprite[y][x]);
	auto vector = (std::vector<attr_t> *) sprite->attr;
	if (!vector->empty())
		vector->back() |= attr;
}

void truncate_attr_from_layer(Layer *layer, unsigned y, unsigned x,
		attr_t attr) {
	if (	y < 0 || y >= layer->yLength ||
			x < 0 || x >= layer->xLength)
		return;
	Sprite *sprite = &(layer->sprite[y][x]);
	auto vector = (std::vector<attr_t> *) sprite->attr;
	if (!vector->empty())
		vector->back() &= ~attr;
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
}
