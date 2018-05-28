#include <limits.h>
#include "stage.h"

Stage *init_stage(void) {
	Stage *stage = malloc(sizeof(Stage));
	stage->backstage = NULL;
	stage->depth = 0; stage->maxDepth = 0;
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
	if (stage && stage->depth > newSc && newSc > 0) {
		stage->currentScene->departure(stage->currentScene->props);
		stage->currentScene = stage->scene[newSc];
		stage->currentScene->arrival(stage->currentScene->props, newSc);
	}
}

// creates a new scene on the stage and returns a pointer to said new layer
void add_scene_to_stage(Stage *stage, UpdateFn upd, KeyboardFn kb,
		ArrivalFn arrival, DepartureFn departure) {
	stage->depth++;
	if (stage->depth >= stage->maxDepth) {
		stage->maxDepth = stage->depth * 2;
		stage->scene = realloc(stage->scene, sizeof(Scene *) * stage->maxDepth);
	}
	stage->scene[stage->depth - 1] =
		init_scene(upd, kb, arrival, departure);
	stage->scene[stage->depth - 1]->props->backstage =
		&(stage->backstage);
	if (stage->currentScene == NULL) {
		stage->currentScene = stage->scene[stage->depth - 1];
	}
}

void remove_scene_from_stage(Stage *stage) {
	if (stage->depth) {
		free_scene(stage->scene[stage->depth - 1]);
		stage->depth--;
		if (stage->depth * 4 < stage->maxDepth) {
			stage->maxDepth = stage->depth * 2;
			if (stage->maxDepth == 0) {
				free(stage->scene);
				stage->scene = NULL;
				stage->depth = 0; // so the analyser will stop bugging me
			} else {
				stage->scene = realloc(stage->scene,
						sizeof(Scene *) * stage->maxDepth);
			}
		}
	}
}
