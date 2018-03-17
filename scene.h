#ifndef SCENE_H
#define SCENE_H
#include <ncurses.h>
#include "screen.h"

//struct Game;

typedef struct Props {
	void *data;
	Screen *screen;
	short changeScene;
} Props;

typedef void (*UpdateFn)(Props *props);
typedef	void (*KeyboardFn)(Props *props, int ch);
typedef	void (*EntryFn)(Props *props);
typedef	void (*ExitFn)(Props *props);

typedef struct Scene {
	Props *props;

	UpdateFn update;
	KeyboardFn keyboard;
	EntryFn entry;
	ExitFn exit;
} Scene;

Scene *init_scene(UpdateFn upd, KeyboardFn kb, EntryFn entry, ExitFn exit);
void free_scene(Scene *scene);
Props *init_props(void);
void free_props(Props *props);

int activate_button(short y, short x, Props *props);
void request_sc_change(Props *props, short newSC);

#endif
