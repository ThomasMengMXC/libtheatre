#ifndef SCENE_H
#define SCENE_H
#include <ncurses.h>
#include "scene_struct.h"
#include "props.h"

Scene *init_scene(UpdateFn upd, KeyboardFn kb, EntryFn entry, ExitFn exit);
void free_scene(Scene *scene);

#endif
