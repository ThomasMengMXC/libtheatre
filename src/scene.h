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

#ifdef __cplusplus
extern "C" {
#endif
Scene *init_scene(UpdateFn upd, KeyboardFn kb,
		ArrivalFn arrival, DepartureFn departure);
void free_scene(Scene *scene);
#ifdef __cplusplus
}
#endif

#endif
