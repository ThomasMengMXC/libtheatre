#ifndef LAYER_STRUCT_H
#define LAYER_STRUCT_H

#include "sprite_struct.h"

typedef struct {
	char draw; // has this layer been updated
	char visibility;
	short yOffset, xOffset;
	short yLength, xLength;
	Sprite **sprite;
} Layer;

#endif
