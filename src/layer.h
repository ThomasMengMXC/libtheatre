#ifndef LAYER_H
#define LAYER_H

#include <stdlib.h>
#include "sprite.h"
#include "vector2D.h"

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
void mv_layer_relative(Layer *layer, short y, short x);
void mv_layer_absolute(Layer *layer, short y, short x);
void refresh_layer(Layer *layer);


// ADDING/REMOVING TO/FROM LAYERS ----------------------------------------

void add_colour_to_layer(Layer *layer, short y, short x, Colour colour);
void remove_colour_from_layer(Layer *layer, short y, short x);

void add_icon_to_layer(Layer *layer, short y, short x, char *icon, size_t n);
void remove_icon_from_layer(Layer *layer, short y, short x, size_t n);

void add_button_to_layer(Layer *layer, short y, short x, Button button);
void remove_button_from_layer(Layer *layer, short y, short x);

void add_hover_to_layer(Layer *layer, short y, short x, Hover hover);
void remove_hover_from_layer(Layer *layer, short y, short x);

void layer_swap(Layer **layer1, Layer **layer2);
void layer_memory_swap(Layer *layer1, Layer *layer2);

#endif
