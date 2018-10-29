#ifndef LAYER_H
#define LAYER_H

#include <curses.h>
#include "sprite.h"
#include "updateStack.h"

typedef struct Layer {
	char visibility;
	char fillSize;
	short yOffset, xOffset;
	unsigned yLength, xLength;
	Sprite **sprite;
	UpdateStack *update;
} Layer;

Layer *init_layer(char fillSize, short yOffset, short xOffset,
		unsigned yLength, unsigned xLength);
void free_layer(Layer *layer);
void clear_layer(Layer *layer);
void resize_layer(Layer *layer, unsigned yLength, unsigned xLength);
void mv_layer_relative(Layer *layer, unsigned y, unsigned x);
void mv_layer_absolute(Layer *layer, unsigned y, unsigned x);
void refresh_layer(Layer *layer);


// ADDING/REMOVING TO/FROM LAYERS ----------------------------------------

void add_colour_to_layer(Layer *layer, unsigned y, unsigned x, Colour colour);
void remove_colour_from_layer(Layer *layer, unsigned y, unsigned x);

void add_icon_to_layer(Layer *layer, unsigned y, unsigned x,
		const char *icon, size_t n);
void remove_icon_from_layer(Layer *layer, unsigned y, unsigned x, size_t n);

void add_button_to_layer(Layer *layer, unsigned y, unsigned x, Button button);
void remove_button_from_layer(Layer *layer, unsigned y, unsigned x);

void add_hover_to_layer(Layer *layer, unsigned y, unsigned x, Hover hover);
void remove_hover_from_layer(Layer *layer, unsigned y, unsigned x);

void add_attr_to_layer(Layer *layer, unsigned y, unsigned x, attr_t attr);
void remove_attr_from_layer(Layer *layer, unsigned y, unsigned x);
void append_attr_to_layer(Layer *layer, unsigned y, unsigned x, attr_t attr);
void truncate_attr_from_layer(Layer *layer, unsigned y, unsigned x,
		attr_t attr);

// Misc-----------------------
void layer_swap(Layer **layer1, Layer **layer2);
void layer_memory_swap(Layer *layer1, Layer *layer2);

#endif
