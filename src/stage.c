#include <stdlib.h>
#include "stage.h"

Stage *init_stage(void) {
	Stage *stage = malloc(sizeof(Stage));
	stage->depth = 0;
	stage->maxDepth = 0;
	stage->scene = NULL;
	stage->currentScene = NULL;
	return stage;
}

void free_stage(Stage *stage) {
	while (stage->depth) {
		remove_scene_from_stage(stage);
	}
	free(stage);
}


void scene_change(Stage *stage, short newSc) {
	if (stage && stage->depth > newSc) {
		stage->currentScene->exit(stage->currentScene->props);
		stage->currentScene = stage->scene[newSc];
		stage->currentScene->entry(stage->currentScene->props);
	}
	return;
}

// creates a new scene on the stage and returns a pointer to said new layer
void add_scene_to_stage(Stage *stage, UpdateFn upd, KeyboardFn kb,
		EntryFn entry, ExitFn exit) {
	stage->depth++;
	if (stage->depth >= stage->maxDepth) {
		stage->maxDepth = (stage->maxDepth + 1) * 2;
		stage->scene = realloc(stage->scene, sizeof(Scene *) * stage->maxDepth);
	}
	stage->scene[stage->depth - 1] = init_scene(upd, kb, entry, exit);
	if (stage->currentScene == NULL) {
		stage->currentScene = stage->scene[stage->depth - 1];
	}
	return;
}

void remove_scene_from_stage(Stage *stage) {
	if (stage->depth) {
		free_scene(stage->scene[stage->depth - 1]);
		stage->depth--;
		while (stage->depth < stage->maxDepth) {
			stage->maxDepth /= 2;
			if (stage->maxDepth == 0) {
				free(stage->scene);
			} else {
				stage->scene = realloc(stage->scene,
						sizeof(Scene *) * stage->maxDepth);
			}
		}
	}
	return;
}
