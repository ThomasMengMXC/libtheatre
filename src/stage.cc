#include <vector>
#include "stage.h"

extern "C" {
Stage *init_stage(void *backstage) {
	Stage *stage = new Stage();
	stage->backstage = backstage;
	stage->scene = (void *) new std::vector<Scene *>();
	stage->currentScene = NULL;
	return stage;
}

void free_stage(Stage *stage) {
	auto scenes = (std::vector<Scene *> *) stage->scene;
	while (!scenes->empty()) {
		remove_scene_from_stage(stage);
	}
	delete scenes;
	delete stage;
}


void scene_change(Stage *stage, short newSc) {
	auto scenes = (std::vector<Scene *> *) stage->scene;
	if (stage && (short) scenes->size() > newSc && newSc > 0) {
		stage->currentScene->departure(stage->currentScene->props);
		stage->currentScene = scenes->at(newSc);
		stage->currentScene->arrival(stage->currentScene->props, newSc);
	}
}

// creates a new scene on the stage and returns a pointer to said new layer
void add_scene_to_stage(Stage *stage, UpdateFn upd, KeyboardFn kb,
		ArrivalFn arrival, DepartureFn departure) {
	auto scenes = (std::vector<Scene *> *) stage->scene;
	scenes->push_back(init_scene(upd, kb, arrival, departure));
	scenes->back()->props->backstage = stage->backstage;
	if (stage->currentScene == NULL) {
		stage->currentScene = scenes->back();
	}
}

void remove_scene_from_stage(Stage *stage) {
	auto scenes = (std::vector<Scene *> *) stage->scene;
	if (!scenes->empty()) {
		free_scene(scenes->back());
		scenes->pop_back();
	}
}
}
