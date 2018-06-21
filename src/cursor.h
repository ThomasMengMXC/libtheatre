#ifndef CURSOR_H
#define CURSOR_H

#include <stdint.h>

typedef struct Cursor {
	char canClick;
	unsigned yPos, xPos;
	unsigned yOld, xOld;
	char *icon;
} Cursor;

Cursor *init_cursor(void);
void free_cursor(Cursor *cursor);

#endif
