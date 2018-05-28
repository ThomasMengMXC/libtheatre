#include "props.h"

Props *init_props(void) {
	Props *props = malloc(sizeof(Props));
	props->data = NULL;
	props->screen = NULL;
	props->backstage = NULL;
	return props;
}

void free_props(Props *props) {
	free(props);
}

void mv_cursor_relative(Props *props, short y, short x) {
	Cursor *cursor = props->screen->cursor;
	if (cursor->yPos + y >= props->screen->yLength
			|| cursor->yPos + y < 0) return;
	if (cursor->xPos + x >= props->screen->xLength
			|| cursor->xPos + x < 0) return;
	cursor->yPos += y;
	cursor->xPos += x;
}

void mv_cursor_absolute(Props *props, short y, short x) {
	Cursor *cursor = props->screen->cursor;
	if (cursor->yPos + y >= props->screen->yLength
			|| cursor->yPos + y < 0) return;
	if (cursor->xPos + x >= props->screen->xLength
			|| cursor->xPos + x < 0) return;
	cursor->yPos = y;
	cursor->xPos = x;
}

int activate_button(Props *props, short y, short x) {
	Button button = get_button(props->screen, y, x);
	if (button == NULL) return 0;
	button(props, y, x);
	return 0;
}

int activate_hover(Props *props, char start, short y, short x) {
	Hover hover = get_hover(props->screen, y, x);
	if (hover == NULL) return 0;
	hover(props, start, y, x);
	return 0;
}
