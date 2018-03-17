#include <stdlib.h>
#include "scene.h"

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

Props *init_props(void) {
	Props *props = malloc(sizeof(Props));
	props->changeScene = -1;
	return props;
}

void free_props(Props *props) {
	free(props);
}

// Returns 1 if nothing is activated, 0 otherwise
int activate_button(short y, short x, Props *props) {
	short buttonLayer = props->screen->depth;
	while (1) {
		if (buttonLayer < 1) {
			break;
		}
		Layer *lyr = props->screen->layer[buttonLayer - 1];
		buttonLayer--;
		short yRelative = y - lyr->yOffset;
		short xRelative = x - lyr->xOffset;
		if (lyr->visibility == false ||
				yRelative < 0 || yRelative >= lyr->yLength ||
				xRelative < 0 || xRelative >= lyr->xLength) {
			continue;
		}
		Sprite *sprite = lyr->sprite[yRelative] + xRelative;
		if (sprite->buttonDepth == 0) {
			continue;
		}
		sprite->button[sprite->colourDepth - 1](props);
		break;
	}
	return 0;
}

void request_sc_change(Props *props, short newSc) {
	props->changeScene = newSc;
}
