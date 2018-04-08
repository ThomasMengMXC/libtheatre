#ifndef PROPS_H
#define PROPS_H

#include <ncurses.h> // needed for the keyboard inputs
#include "screen.h"

typedef struct Props {
	void *data;
	Screen *screen;
	void **backstage;
} Props;

typedef void (*UpdateFn)(Props *props);
typedef	int (*KeyboardFn)(Props *props, int ch);
typedef	void (*ArrivalFn)(Props *props);
typedef	void (*DepartureFn)(Props *props);

Props *init_props(void);
void free_props(Props *props);

int activate_button(short y, short x, Props *props);

#endif
