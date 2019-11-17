#include <stdlib.h>
#include <curses.h>
#include "screen.h"

static void paint_colour(Screen *scr, short y, short x);
static void set_attr(Screen *scr, short y, short x);
static void draw_icon(Screen *scr, short y, short x);
static Layer *remove_layer_from_scr(Screen *scr);

Screen *init_screen(short yLength, short xLength) {
	Screen *scr = malloc(sizeof(Screen));
	if (yLength == 0)
		yLength = (getmaxy(stdscr) | LINES);
	if (xLength == 0)
		xLength = (getmaxx(stdscr) | COLS) / 2;
	scr->yLength = yLength; scr->xLength = xLength;
	scr->layers = init_uVec(sizeof(Layer *));
	scr->update = init_update_stack(yLength, xLength);
	scr->cursor = init_cursor();
	return scr;
}

void free_screen(Screen *scr) {
	free_cursor(scr->cursor);
	while (scr->layers->length != 0)
		remove_layer_from_scr(scr);
	free_update_stack(scr->update, scr->yLength, scr->xLength);
	free_uVec(scr->layers);
	free(scr);
}

int draw_screen(Screen *scr) {
	unsigned y, x, update;
	uVec *stack = scr->update->stack;
	Coordinate vector;
	update = 0;
	while (stack->length != 0) {
		update = 1;
		vector = vector2D_pop(scr->update);
		y = vector.y; x = vector.x;

		paint_colour(scr, y, x);

		set_attr(scr, y, x);

		draw_icon(scr, y, x);
	}
	if (update)
		refresh();
	return 0;
}

void clear_screen(Screen *scr) {
	for (unsigned i = 0; i < scr->layers->length; i++) {
		clear_layer(*(Layer **) uVec_get(scr->layers, i));
	}
}

void resize_screen(Screen *scr) {
	Cursor *cursor = scr->cursor;
	free_update_stack(scr->update, scr->yLength, scr->xLength);
	if ((scr->yLength = (getmaxy(stdscr) | LINES)) == 0)
		scr->yLength = 1;
	if ((scr->xLength = (getmaxx(stdscr) | COLS) / 2) == 0)
		scr->xLength = 1;
	if (cursor->yPos > scr->yLength)
		cursor->yPos = scr->yLength - 1;
	if (cursor->xPos > scr->xLength)
		cursor->xPos = scr->xLength - 1;
	scr->update = init_update_stack(scr->yLength, scr->xLength);
	for (unsigned i = 0; i < scr->layers->length; i++) {
		Layer *layer = *(Layer **) uVec_get(scr->layers, i);
		layer->update = scr->update;
		if (layer->fillSize)
			resize_layer(layer, scr->yLength, scr->xLength);
		refresh_layer(layer);
	}
}

// creates a new layer on the screen and returns a pointer to said new layer
Layer *add_layer_to_scr(Screen *scr, short yOffset, short xOffset,
		short yLength, short xLength) {
	char fillSize = 0;
	if (yLength == 0) {
		yLength = scr->yLength;
		fillSize = 1;
	}
	if (xLength == 0) {
		xLength = scr->xLength;
		fillSize = 1;
	}
	Layer *l = init_layer(fillSize, yOffset, xOffset, yLength, xLength);
	uVec_push(scr->layers, &l);
	(*(Layer **)uVec_get_back(scr->layers))->update = scr->update;
	return (*(Layer **)uVec_get_back(scr->layers));
}

// Returns 1 if nothing is activated, 0 otherwise
Button get_button(Screen *scr, short y, short x) {
	for (unsigned i = 0; i < scr->layers->length; i++) {
		Layer *layer = *(Layer **) uVec_get(scr->layers, i);
		long yRelative = y - layer->yOffset;
		long xRelative = x - layer->xOffset;
		if (layer->visibility == 0 ||
				yRelative < 0 || yRelative >= layer->yLength ||
				xRelative < 0 || xRelative >= layer->xLength)
			continue;
		Sprite *sprite = &(layer->sprite[yRelative][xRelative]);
		if (sprite->button->length == 0)
			continue;
		return (*(Button *)uVec_get_back(sprite->button));
	}
	return NULL;
}

// Returns 1 if nothing is activated, 0 otherwise
Hover get_hover(Screen *scr, short y, short x) {
	for (unsigned i = 0; i < scr->layers->length; i++) {
		Layer *layer = *(Layer **) uVec_get(scr->layers, i);
		unsigned yRelative = y - layer->yOffset;
		unsigned xRelative = x - layer->xOffset;
		if (layer->visibility == 0 ||
				yRelative < 0 || yRelative >= layer->yLength ||
				xRelative < 0 || xRelative >= layer->xLength)
			continue;
		Sprite *sprite = &(layer->sprite[yRelative][xRelative]);
		if (sprite->hover->length == 0)
			continue;
		return (*(Hover *)uVec_get_back(sprite->hover));
	}
	return NULL;
}

// STATIC -----------------
static void paint_colour(Screen *scr, short y, short x) {
	unsigned layerDepth = scr->layers->length;
	float block = 1.0;
	float r0 = 0;
	float g0 = 0;
	float b0 = 0;
	Colour col;
	uVec *colour;
	unsigned colourDepth = 0;
	while (layerDepth) {
		Layer *layer = *(Layer **) uVec_get(scr->layers, layerDepth - 1);
		layerDepth--;
		long yRelative = y - layer->yOffset;
		long xRelative = x - layer->xOffset;
		if (layer->visibility == 0 ||
				yRelative < 0 || yRelative >= layer->yLength ||
				xRelative < 0 || xRelative >= layer->xLength)
			continue;
		Sprite *sprite = &(layer->sprite[yRelative][xRelative]);
		colour = sprite->colour;
		colourDepth = colour->length;
		while (colourDepth) {
			col = *(Colour *) uVec_get(colour, colourDepth - 1);
			r0 += col.r * block * col.a / 255;
			g0 += col.g * block * col.a / 255;
			b0 += col.b * block * col.a / 255;
			block = block * (255 - col.a) / 255;
			colourDepth--;
		}
	}
	attron(COLOR_PAIR(rgb_to_term256(r0, g0, b0) + 1));
}

static void set_attr(Screen *scr, short y, short x) {
	for (unsigned i = 0; i < scr->layers->length; i++) {
		Layer *layer = *(Layer **) uVec_get(scr->layers, i);
		long yRelative = y - layer->yOffset;
		long xRelative = x - layer->xOffset;
		if (layer->visibility == 0 ||
				yRelative < 0 || yRelative >= layer->yLength ||
				xRelative < 0 || xRelative >= layer->xLength)
			continue;
		Sprite *sprite = &(layer->sprite[yRelative][xRelative]);
		if (sprite->attr->length == 0)
			continue;
		attron(*(attr_t *) uVec_get_back(sprite->attr));
		break;
	}
}

static void draw_icon(Screen *scr, short y, short x) {
	char drawn = 0;
	for (unsigned i = 0; i < scr->layers->length; i++) {
		Layer *layer = *(Layer **) uVec_get(scr->layers, i);
		long yRelative = y - layer->yOffset;
		long xRelative = x - layer->xOffset;
		if (layer->visibility == 0 ||
				yRelative < 0 || yRelative >= layer->yLength ||
				xRelative < 0 || xRelative >= layer->xLength)
			continue;
		Sprite *sprite = &(layer->sprite[yRelative][xRelative]);
		if (sprite->icon->length == 0)
			continue;
		mvaddnstr(y, x * 2, (char *) uVec_get_back(sprite->icon), 2);
		drawn = 1;
		break;
	}
	if (!drawn) {
		mvaddnstr(y, x * 2, "  ", 2);
	}
}

static Layer *remove_layer_from_scr(Screen *scr) {
	// should probably remove this
	Layer *oldPtr = *(Layer **) uVec_get_back(scr->layers);
	free_layer(oldPtr);
	uVec_pop(scr->layers);
	return oldPtr;
}
