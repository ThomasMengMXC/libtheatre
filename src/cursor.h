#ifndef CURSOR_H
#define CURSOR_H

typedef struct {
	char canClick;
	unsigned char yPos, xPos;
	unsigned char yOld, xOld;
	char *icon;
} Cursor;

Cursor *init_cursor(void);
void free_cursor(Cursor *cursor);

#endif
