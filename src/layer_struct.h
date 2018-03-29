#ifndef LAYER_STRUCT_H
#define LAYER_STRUCT_H

#include "sprite_struct.h"

typedef struct {
	char visibility; // has this layer been updated
	short yOffset, xOffset;
	short yLength, xLength;
	Sprite **sprite;
	char ***update;
} Layer;

#endif
