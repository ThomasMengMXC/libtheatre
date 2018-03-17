#ifndef STAGE_H
#define STAGE_H

#include "scene.h"

typedef struct Stage{
	void *backstage;
	short depth;
	Scene **scene;
	Scene *currentScene;
} Stage;

Stage *init_stage(void);
void free_stage(Stage *stage);
void scene_change(Stage *stage, short newSc);
void stage_exit(Stage *stage);

void add_scene_to_stage(Stage *stage, UpdateFn upd, KeyboardFn kb,
		EntryFn entry, ExitFn exit);
void remove_scene_from_stage(Stage *stage);

#endif
