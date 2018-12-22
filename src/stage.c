#include <stdlib.h>
#include "stage.h"

Stage *init_stage(void *backstage) {
	Stage *stage = malloc(sizeof(Stage));
	stage->backstage = backstage;
	stage->scenes = init_uVec(sizeof(Scene *));
	stage->currentScene = NULL;
	return stage;
}

void free_stage(Stage *stage) {
	while (stage->scenes->length != 0) {
		remove_scene_from_stage(stage);
	}
	free_uVec(stage->scenes);
	free(stage);
}


void scene_change(Stage *stage, short newSc) {
	if (stage && (short) stage->scenes->length > newSc && newSc > 0) {
		stage->currentScene->departure(stage->currentScene->props);
		stage->currentScene = *(Scene **) uVec_get(stage->scenes, newSc);
		stage->currentScene->arrival(stage->currentScene->props, newSc);
	}
}

// creates a new scene on the stage and returns a pointer to said new layer
void add_scene_to_stage(Stage *stage, UpdateFn upd, KeyboardFn kb,
		ArrivalFn arrival, DepartureFn departure) {
	Scene *sc = init_scene(upd, kb, arrival, departure);
	uVec_push(stage->scenes, &sc);
	(*(Scene **) uVec_get_back(stage->scenes))->props->backstage
		= stage->backstage;
	if (stage->currentScene == NULL) {
		stage->currentScene = *(Scene **) uVec_get_back(stage->scenes);
	}
}

void remove_scene_from_stage(Stage *stage) {
	if (stage->scenes->length != 0) {
		free_scene(*(Scene **) uVec_get_back(stage->scenes));
		uVec_pop(stage->scenes);
	}
}
