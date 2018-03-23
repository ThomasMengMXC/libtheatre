#ifndef STAGE_STRUCT_H
#define STAGE_STRUCT_H

#include "scene_struct.h"

typedef struct Stage{
	void *backstage;
	short depth;
	short maxDepth;
	Scene **scene;
	Scene *currentScene;
} Stage;

#endif
