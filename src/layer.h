#ifndef LAYER_H
#define LAYER_H
#include <ncurses.h>
#include "sprite.h"

typedef struct {
	char draw; // has this layer been updated
	char visibility;
	short yOffset, xOffset;
	short yLength, xLength;
	Sprite **sprite;
} Layer;

Layer *init_layer(short yOffset, short xOffset, short yLength, short xLength);
void free_layer(Layer *layer);

int activate_colour(Layer **layer, short y, short x, char colourLayer);
int deactivate_colour(Layer **layer, short y, short x, char colourLayer);
int draw_icon(Layer **layer, short y, short x, char iconLayer);

void add_colour_to_layer(Layer *layer, short y, short x, Colour colour);
void remove_colour_from_layer(Layer *layer, short y, short x);

void add_icon_to_layer(Layer *layer, short y, short x, char *icon);
void remove_icon_from_layer(Layer *layer, short y, short x);

void add_button_to_layer(Layer *layer, short y, short x, Button button);
void remove_button_from_layer(Layer *layer, short y, short x);

void layer_swap(Layer **layer1, Layer **layer2);
void layer_memory_swap(Layer *layer1, Layer *layer2);

#endif
