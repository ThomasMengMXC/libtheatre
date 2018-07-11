#include "scene.h"

extern "C" {
Scene *init_scene(UpdateFn upd, KeyboardFn kb,
		ArrivalFn arrival, DepartureFn departure) {
	Scene *scene = new Scene();
	scene->props = init_props();
	scene->update = upd; scene->keyboard = kb; //update & kb hooks during run
	// entry/exit hooks
	scene->arrival = arrival; scene->departure = departure;
	return scene;
}

void free_scene(Scene *scene) {
	free_props(scene->props);
	delete scene;
}
}
