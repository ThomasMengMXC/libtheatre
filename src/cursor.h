#ifndef CURSOR_H
#define CURSOR_H

#include <stdint.h>

typedef struct Cursor {
	char canClick;
	uint16_t yPos, xPos;
	uint16_t yOld, xOld;
	char *icon;
} Cursor;

Cursor *init_cursor(void);
void free_cursor(Cursor *cursor);

#endif
