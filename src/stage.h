#ifndef STAGE_H
#define STAGE_H

#include "scene.h"

typedef struct Stage{
	void *backstage;
	void *scene; // is std::vector<Scene *> *
	Scene *currentScene;
} Stage;

#ifdef __cplusplus
extern "C" {
#endif
Stage *init_stage(void *backstage);
void free_stage(Stage *stage);
void scene_change(Stage *stage, short newSc);
void stage_exit(Stage *stage);

void add_scene_to_stage(Stage *stage, UpdateFn upd, KeyboardFn kb,
		ArrivalFn arrival, DepartureFn departure);
void remove_scene_from_stage(Stage *stage);
#ifdef __cplusplus
}
#endif

#endif
