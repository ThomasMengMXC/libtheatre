#include <stdlib.h>
#include "cursor.h"

Cursor *init_cursor(void) {
	Cursor *cursor = malloc(sizeof(Cursor));
	cursor->canClick = 1;
	cursor->icon = NULL;
	cursor->xPos = 0;				cursor->yPos = 0;
	cursor->xOld = cursor->xPos;	cursor->yOld = cursor->yPos;
	return cursor;
}

void free_cursor(Cursor *cursor) {
	free(cursor);
}
