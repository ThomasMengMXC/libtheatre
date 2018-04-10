#ifndef LAYER_H
#define LAYER_H

#include "sprite.h"
#include "vector2D.h"

typedef struct {
	char visibility; // has this layer been updated
	short yOffset, xOffset;
	short yLength, xLength;
	Sprite **sprite;
	Vector2DStack *update;
} Layer;

Layer *init_layer(short yOffset, short xOffset, short yLength, short xLength);
void free_layer(Layer *layer);

void clear_layer(Layer *layer);

int activate_colour(Layer **layer, short y, short x, char colourLayer);
int deactivate_colour(Layer **layer, short y, short x, char colourLayer);
int draw_icon(Layer **layer, short y, short x, char iconLayer);

void add_colour_to_layer(Layer *layer, short y, short x, short term, 
		uint8_t r, uint8_t g, uint8_t b);
void remove_colour_from_layer(Layer *layer, short y, short x);

void add_icon_to_layer(Layer *layer, short y, short x, char *icon);
void remove_icon_from_layer(Layer *layer, short y, short x);

void add_button_to_layer(Layer *layer, short y, short x, Button button);
void remove_button_from_layer(Layer *layer, short y, short x);

void add_hover_to_layer(Layer *layer, short y, short x, Hover hover);
void remove_hover_from_layer(Layer *layer, short y, short x);

void layer_swap(Layer **layer1, Layer **layer2);
void layer_memory_swap(Layer *layer1, Layer *layer2);

#endif
