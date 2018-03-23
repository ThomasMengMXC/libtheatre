#ifndef SCENE_STRUCT_H
#define SCENE_STRUCT_H

#include "props_struct.h"

typedef struct Scene {
	Props *props;

	UpdateFn update;
	KeyboardFn keyboard;
	EntryFn entry;
	ExitFn exit;
} Scene;

#endif
