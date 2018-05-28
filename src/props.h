#ifndef PROPS_H
#define PROPS_H

#include <curses.h> // needed for the keyboard inputs
#include "screen.h"

typedef struct Props {
	void *data;
	Screen *screen;
	void **backstage;
} Props;

typedef void (*UpdateFn)(Props *props);
typedef	int (*KeyboardFn)(Props *props, int ch);
typedef	void (*ArrivalFn)(Props *props, int sceneNumber);
typedef	void (*DepartureFn)(Props *props);

Props *init_props(void);
void free_props(Props *props);

void mv_cursor_relative(Props *props, short y, short x);
void mv_cursor_absolute(Props *props, short y, short x);

int activate_button(Props *props, short y, short x);
int activate_hover(Props *props, char start, short y, short x);

#endif
