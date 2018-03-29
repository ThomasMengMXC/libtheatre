#ifndef PROPS_STRUCT_H
#define PROPS_STRUCT_H

#include "screen_struct.h"

typedef struct Props {
	void *data;
	Screen *screen;
	short changeScene;
	char quit;
	void **backstage;
} Props;

typedef void (*UpdateFn)(Props *props);
typedef	void (*KeyboardFn)(Props *props, int ch);
typedef	void (*EntryFn)(Props *props);
typedef	void (*ExitFn)(Props *props);

#endif
