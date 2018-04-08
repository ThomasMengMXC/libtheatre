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

// Returns 1 if nothing is activated, 0 otherwise
int activate_button(short y, short x, Props *props) {
	short buttonLayer = props->screen->depth;
	while (1) {
		if (buttonLayer < 1) {
			break;
		}
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
		if (sprite->buttonDepth == 0) {
			continue;
		}
		sprite->button[sprite->colourDepth - 1](props, y, x);
		break;
	}
	return 0;
}
