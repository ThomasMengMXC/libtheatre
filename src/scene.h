#ifndef SCENE_H
#define SCENE_H
#include "props.h"

typedef struct Scene {
	Props *props;

	UpdateFn update;
	KeyboardFn keyboard;
	ArrivalFn arrival;
	DepartureFn departure;
} Scene;

Scene *init_scene(UpdateFn upd, KeyboardFn kb,
		ArrivalFn arrival, DepartureFn departure);
void free_scene(Scene *scene);

#endif
