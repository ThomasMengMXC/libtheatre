#ifndef SCREEN_STRUCT_H
#define SCREEN_STRUCT_H

#include "layer_struct.h"

typedef struct {
	short depth;
	short yLength, xLength;
	Layer **layer;
} Screen;

#endif
