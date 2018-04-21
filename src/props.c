#include <stdlib.h>
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
	//activate_hover(props, 0, cursor->yPos, cursor->xPos);
	cursor->yPos += y;
	cursor->xPos += x;
	//activate_hover(props, 1, cursor->yPos, cursor->xPos);
	return;
}

void mv_cursor_absolute(Props *props, short y, short x) {
	Cursor *cursor = props->screen->cursor;
	if (cursor->yPos + y >= props->screen->yLength
			|| cursor->yPos + y < 0) return;
	if (cursor->xPos + x >= props->screen->xLength
			|| cursor->xPos + x < 0) return;
	activate_hover(props, 0, cursor->yPos, cursor->xPos);
	cursor->yPos = y;
	cursor->xPos = x;
	activate_hover(props, 1, cursor->yPos, cursor->xPos);
	return;
}

// Returns 1 if nothing is activated, 0 otherwise
int activate_button(Props *props, short y, short x) {
	short buttonLayer = props->screen->depth;
	while (buttonLayer) {
		Layer *lyr = props->screen->layer[buttonLayer - 1];
		buttonLayer--;
		short yRelative = y - lyr->yOffset;
		short xRelative = x - lyr->xOffset;
		if (lyr->visibility == 0 ||
				yRelative < 0 || yRelative >= lyr->yLength ||
				xRelative < 0 || xRelative >= lyr->xLength) {
			continue;
		}
		Sprite *sprite = lyr->sprite[yRelative] + xRelative;
		if (sprite->buttonDepth == 0) continue;
		sprite->button[sprite->colourDepth - 1](props, lyr, y, x);
		break;
	}
	return 0;
}

// Returns 1 if nothing is activated, 0 otherwise
int activate_hover(Props *props, char start,  short y, short x) {
	short hoverLayer = props->screen->depth;
	while (hoverLayer) {
		Layer *lyr = props->screen->layer[hoverLayer - 1];
		hoverLayer--;
		short yRelative = y - lyr->yOffset;
		short xRelative = x - lyr->xOffset;
		if (lyr->visibility == 0 ||
				yRelative < 0 || yRelative >= lyr->yLength ||
				xRelative < 0 || xRelative >= lyr->xLength) {
			continue;
		}
		Sprite *sprite = lyr->sprite[yRelative] + xRelative;
		if (sprite->hoverDepth == 0) continue;
		sprite->hover[sprite->colourDepth - 1](props, lyr, start, y, x);
		break;
	}
	return 0;
}
