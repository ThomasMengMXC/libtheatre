#include <curses.h>

#include "screen.h"

Screen *init_screen(short yLength, short xLength) {
	Screen *scr = malloc(sizeof(Screen));
	scr->depth = 0; scr->maxDepth = 0;
	if (yLength == 0) yLength = (getmaxy(stdscr) | LINES);
	if (xLength == 0) xLength = (getmaxx(stdscr) | COLS) / 2;
	scr->yLength = yLength; scr->xLength = xLength;
	scr->layer = NULL;
	scr->update = init_update_stack(yLength, xLength);
	scr->cursor = init_cursor();
	return scr;
}

void free_screen(Screen *scr) {
	free_cursor(scr->cursor);
	while (scr->depth) {
		remove_layer_from_scr(scr);
	}
	free_update_stack(scr->update, scr->yLength, scr->xLength);
	free(scr);
}

int draw_screen(Screen *scr) {
	uint16_t y, x, update;
	Vector2D vector;
	update = 0;
	while (scr->update->depth > 0) {
		update = 1;
		vector = vector2D_pop(scr->update);
		y = vector.y; x = vector.x;

		paint_colour(scr, y, x);

		draw_icon(scr, y, x);
	}
	if (update) refresh();
	return 0;
}

void clear_screen(Screen *scr) {
	uint16_t layerDepth;
	for (layerDepth = scr->depth; layerDepth > 0; layerDepth--) {
		clear_layer(scr->layer[layerDepth - 1]);
	}
}

void resize_screen(Screen *scr) {
	Layer *layer = NULL;
	uint16_t layerDepth;
	Cursor *cursor = scr->cursor;
	free_update_stack(scr->update, scr->yLength, scr->xLength);
	if ((scr->yLength = (getmaxy(stdscr) | LINES)) == 0) scr->yLength = 1;
	if ((scr->xLength = (getmaxx(stdscr) | COLS) / 2) == 0) scr->xLength = 1;
	if (cursor->yPos > scr->yLength) cursor->yPos = scr->yLength - 1;
	if (cursor->xPos > scr->xLength) cursor->xPos = scr->xLength - 1;
	scr->update = init_update_stack(scr->yLength, scr->xLength);
	for (layerDepth = scr->depth; layerDepth > 0; layerDepth--) {
		layer = scr->layer[layerDepth - 1];
		layer->update = scr->update;
		if (layer->fillSize) resize_layer(layer, scr->yLength, scr->xLength);
		refresh_layer(scr->layer[layerDepth - 1]);
	}
}

/* creates a new layer on the screen and returns a pointer to the new layer */
Layer *add_layer_to_scr(Screen *scr, short yOffset, short xOffset,
		short yLength, short xLength) {
	char fillSize = 0;
	scr->depth++;
	if (scr->depth >= scr->maxDepth) {
		scr->maxDepth = scr->depth * 2;
		scr->layer = realloc(scr->layer, sizeof(Layer *) * scr->maxDepth);
	}

	if (yLength == 0) {yLength = scr->yLength; fillSize = 1;}
	if (xLength == 0) {xLength = scr->xLength; fillSize = 1;}
	scr->layer[scr->depth - 1] = init_layer(fillSize, yOffset, xOffset,
			yLength, xLength);
	scr->layer[scr->depth - 1]->update = scr->update;
	return scr->layer[scr->depth - 1];
}

Layer *remove_layer_from_scr(Screen *scr) {
	Layer *oldPtr;
	if (!scr->layer || !scr->depth) return NULL;
	oldPtr = scr->layer[scr->depth - 1];
	free_layer(scr->layer[scr->depth - 1]);
	scr->layer[scr->depth - 1] = NULL;
	scr->depth--;
	while (scr->depth * 2 < scr->maxDepth) {
		scr->maxDepth = scr->depth * 3 / 2;
		if (scr->maxDepth == 0) {
			free(scr->layer);
			scr->layer = NULL;
		} else {
			scr->layer = realloc(scr->layer,
					sizeof(Layer *) * scr->maxDepth);
		}
	}
	return oldPtr;
}

void paint_colour(Screen *scr, short y, short x) {
	short layerDepth = scr->depth;
	Colour col, *colour;
	short yRelative, xRelative;
	Sprite *sprite;
	float r0 = 0, r1 = 1;
	float g0 = 0, g1 = 1;
	float b0 = 0, b1 = 1;
	uint8_t colourDepth = 0;
	while (layerDepth && r1 + g1 + b1 > 0.0001) {
		Layer *lyr = scr->layer[layerDepth - 1];
		layerDepth--;
		yRelative = y - lyr->yOffset;
		xRelative = x - lyr->xOffset;
		if (lyr->visibility == 0 ||
				yRelative < 0 || yRelative >= lyr->yLength ||
				xRelative < 0 || xRelative >= lyr->xLength) {
			continue;
		}
		sprite = lyr->sprite[yRelative] + xRelative;
		colourDepth = sprite->colourDepth;
		colour = sprite->colour;
		while (colourDepth && r1 + g1 + b1 > 0.0001) {
			col = colour[colourDepth - 1];
			r0 += col.r * r1 * col.a / 255; r1 = r1 * (255 - col.a) / 255;
			g0 += col.g * g1 * col.a / 255; g1 = g1 * (255 - col.a) / 255;
			b0 += col.b * b1 * col.a / 255; b1 = b1 * (255 - col.a) / 255;
			colourDepth--;
		}
	}
	attron(COLOR_PAIR(rgb_to_term256(r0, g0, b0) + 1));
}

void draw_icon(Screen *scr, short y, short x) {
	short layerDepth, yRelative, xRelative;
	Sprite *sprite;
	char drawn = 0;
	for (layerDepth = scr->depth; layerDepth > 0; layerDepth--) {
		Layer *lyr = scr->layer[layerDepth - 1];
		yRelative = y - lyr->yOffset;
		xRelative = x - lyr->xOffset;
		if (lyr->visibility == 0 ||
				yRelative < 0 || yRelative >= lyr->yLength ||
				xRelative < 0 || xRelative >= lyr->xLength) {
			continue;
		}
		sprite = lyr->sprite[yRelative] + xRelative;
		if (sprite->iconDepth == 0) continue;
		mvaddnstr(y, x * 2, sprite->icon[sprite->iconDepth - 1], 2);
		drawn = 1;
		break;
	}
	if (!drawn) {
		mvaddnstr(y, x * 2, "  ", 2);
	}
}
