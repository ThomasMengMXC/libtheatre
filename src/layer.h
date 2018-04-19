#ifndef LAYER_H
#define LAYER_H

#include "sprite.h"
#include "vector2D.h"

#define add_to_layer(layer, y, x, object)\
	do { \
	_Generic((object),	uint8_t: add_colour_to_layer, \
						char *: add_icon_to_layer, \
						Button: add_button_to_layer, \
						Hover: add_hover_to_layer)(layer, y, x, object); \
	} while(0)

typedef struct {
	char visibility; // has this layer been updated
	short yOffset, xOffset;
	short yLength, xLength;
	Sprite **sprite;
	UpdateStack *update;
} Layer;

Layer *init_layer(short yOffset, short xOffset, short yLength, short xLength);
void free_layer(Layer *layer);
void clear_layer(Layer *layer);
void mv_layer_relative(Layer *layer, short y, short x);
void mv_layer_absolute(Layer *layer, short y, short x);
void refresh_layer(Layer *layer);

int paint_colour(Layer **layer, short y, short x, char colourLayer);
int draw_icon(Layer **layer, short y, short x, char iconLayer);


// ADDING/REMOVING TO/FROM LAYERS ----------------------------------------

void add_colour_to_layer(Layer *layer, short y, short x, uint8_t colour);
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
