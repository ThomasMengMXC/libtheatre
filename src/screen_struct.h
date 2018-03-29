#ifndef SCREEN_STRUCT_H
#define SCREEN_STRUCT_H

#include "layer_struct.h"

typedef struct {
	short depth, maxDepth;  // The amount of layers that there are
	short yLength, xLength; // Size of the screen
	Layer **layer; // The layers for the screen
	char **update; // Which tile should be drawn
} Screen;

#endif
