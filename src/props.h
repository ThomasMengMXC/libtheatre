#ifndef PROPS_H
#define PROPS_H

#include <ncurses.h>
#include "props_struct.h"

Props *init_props(void);
void free_props(Props *props);

int activate_button(short y, short x, Props *props);
void request_sc_change(Props *props, short newSC);
void quit(Props *props);

#endif
