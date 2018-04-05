#ifndef PROPS_H
#define PROPS_H

#include <ncurses.h> // needed for the keyboard inputs
#include "screen.h"

typedef struct Props {
	void *data;
	Screen *screen;
	short changeScene;
	char quit;
	void **backstage;
} Props;

typedef void (*UpdateFn)(Props *props);
typedef	void (*KeyboardFn)(Props *props, int ch);
typedef	void (*ArrivalFn)(Props *props);
typedef	void (*DepartureFn)(Props *props);

Props *init_props(void);
void free_props(Props *props);

int activate_button(short y, short x, Props *props);
void request_sc_change(Props *props, short newSC);
void quit(Props *props);

#endif
