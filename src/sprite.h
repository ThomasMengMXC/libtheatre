#ifndef SPRITE_STRUCT_H
#define SPRITE_STRUCT_H

#include "sprite_struct.h"

Sprite **init_sprite(short yLength, short xLength);
void free_sprite(short yLength, short xLength, Sprite **sprite);

#endif
