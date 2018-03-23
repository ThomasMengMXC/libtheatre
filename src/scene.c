#include <stdlib.h>
#include "scene.h"
#include "props.h"

Scene *init_scene(UpdateFn upd, KeyboardFn kb, EntryFn entry, ExitFn exit) {
	Scene *scene = malloc(sizeof(Scene));
	scene->props = init_props();
	scene->update = upd; scene->keyboard = kb; //update & kb hooks during run
	scene->entry = entry; scene->exit = exit; //entry/exit hooks
	return scene;
}

void free_scene(Scene *scene) {
	free_props(scene->props);
	free(scene);
}
