extern "C" {
#include <curses.h>
}
#include <vector>
#include <stack>
#include <array>

#include "screen.h"

static void paint_colour(Screen *scr, short y, short x);
static void draw_icon(Screen *scr, short y, short x);

extern "C" {
Screen *init_screen(short yLength, short xLength) {
	Screen *scr = new Screen();
	if (yLength == 0)
		yLength = (getmaxy(stdscr) | LINES);
	if (xLength == 0)
		xLength = (getmaxx(stdscr) | COLS) / 2;
	scr->yLength = yLength; scr->xLength = xLength;
	scr->layer = (void *) new std::vector<Layer *>();
	scr->update = init_update_stack(yLength, xLength);
	scr->cursor = init_cursor();
	return scr;
}

void free_screen(Screen *scr) {
	auto layers = (std::vector<Layer *> *) scr->layer;
	free_cursor(scr->cursor);
	while (!layers->empty())
		remove_layer_from_scr(scr);
	free_update_stack(scr->update, scr->yLength, scr->xLength);
	delete (std::vector<Layer *> *) scr->layer;
	delete scr;
}

int draw_screen(Screen *scr) {
	unsigned y, x, update;
	auto stack = (std::stack<Vector2D> *) scr->update->stack;
	Vector2D vector;
	update = 0;
	while (stack->size() > 0) {
		update = 1;
		vector = vector2D_pop(scr->update);
		y = vector.y; x = vector.x;

		paint_colour(scr, y, x);

		draw_icon(scr, y, x);
	}
	if (update)
		refresh();
	return 0;
}

void clear_screen(Screen *scr) {
	auto layers = (std::vector<Layer *> *) scr->layer;
	for (unsigned i = 0; i < layers->size(); i++) {
		auto layer = layers->at(i);
		clear_layer(layer);
	}
}

void resize_screen(Screen *scr) {
	auto layers = (std::vector<Layer *> *) scr->layer;
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
	for (unsigned i = 0; i < layers->size(); i++) {
		auto layer = layers->at(i);
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
	auto layers = (std::vector<Layer *> *) scr->layer;
	if (yLength == 0) {
		yLength = scr->yLength;
		fillSize = 1;
	}
	if (xLength == 0) {
		xLength = scr->xLength;
		fillSize = 1;
	}
	layers->push_back(init_layer(fillSize, yOffset, xOffset,
			yLength, xLength));
	layers->back()->update = scr->update;
	return layers->back();
}

Layer *remove_layer_from_scr(Screen *scr) {
	auto layers = (std::vector<Layer *> *) scr->layer;
	Layer *oldPtr = layers->back(); // should probably remove this
	free_layer(layers->back());
	layers->pop_back();
	return oldPtr;
}

// Returns 1 if nothing is activated, 0 otherwise
Button get_button(Screen *scr, short y, short x) {
	auto layers = (std::vector<Layer *> *) scr->layer;
	for (unsigned i = 0; i < layers->size(); i++) {
		auto layer = layers->at(i);
		long yRelative = y - layer->yOffset;
		long xRelative = x - layer->xOffset;
		if (layer->visibility == 0 ||
				yRelative < 0 || yRelative >= layer->yLength ||
				xRelative < 0 || xRelative >= layer->xLength)
			continue;
		Sprite *sprite = &(layer->sprite[yRelative][xRelative]);
		auto buttonStack = (std::vector<Button> *) sprite->button;
		if (buttonStack->empty())
			continue;
		return buttonStack->back();
	}
	return NULL;
}

// Returns 1 if nothing is activated, 0 otherwise
Hover get_hover(Screen *scr, short y, short x) {
	auto layers = (std::vector<Layer *> *) scr->layer;
	for (unsigned i = 0; i < layers->size(); i++) {
		auto layer = layers->at(i);
		unsigned yRelative = y - layer->yOffset;
		unsigned xRelative = x - layer->xOffset;
		if (layer->visibility == 0 ||
				yRelative < 0 || yRelative >= layer->yLength ||
				xRelative < 0 || xRelative >= layer->xLength)
			continue;
		Sprite *sprite = &(layer->sprite[yRelative][xRelative]);
		auto hoverStack = (std::vector<Hover> *) sprite->hover;
		if (hoverStack->empty())
			continue;
		return hoverStack->back();
	}
	return NULL;
}
}

// STATIC -----------------
static void paint_colour(Screen *scr, short y, short x) {
	auto layers = (std::vector<Layer *> *) scr->layer;
	unsigned layerDepth = layers->size();
	float r0 = 0, r1 = 1;
	float g0 = 0, g1 = 1;
	float b0 = 0, b1 = 1;
	Colour col;
	std::vector<Colour> *colour;
	unsigned colourDepth = 0;
	while (layerDepth && r1 + g1 + b1 > 0.0001) {
		Layer *layer = layers->at(layerDepth - 1);
		layerDepth--;
		long yRelative = y - layer->yOffset;
		long xRelative = x - layer->xOffset;
		if (layer->visibility == 0 ||
				yRelative < 0 || yRelative >= layer->yLength ||
				xRelative < 0 || xRelative >= layer->xLength)
			continue;
		Sprite *sprite = &(layer->sprite[yRelative][xRelative]);
		colour = (std::vector<Colour> *) sprite->colour;
		colourDepth = colour->size();
		while (colourDepth && r1 + g1 + b1 > 0.0001) {
			col = colour->at(colourDepth - 1);
			r0 += col.r * r1 * col.a / 255; r1 = r1 * (255 - col.a) / 255;
			g0 += col.g * g1 * col.a / 255; g1 = g1 * (255 - col.a) / 255;
			b0 += col.b * b1 * col.a / 255; b1 = b1 * (255 - col.a) / 255;
			colourDepth--;
		}
	}
	attron(COLOR_PAIR(rgb_to_term256(r0, g0, b0) + 1));
}

static void draw_icon(Screen *scr, short y, short x) {
	auto layers = (std::vector<Layer *> *) scr->layer;
	char drawn = 0;
	for (short layerDepth = layers->size(); layerDepth > 0; layerDepth--) {
		Layer *layer = layers->at(layerDepth - 1);
		long yRelative = y - layer->yOffset;
		long xRelative = x - layer->xOffset;
		if (layer->visibility == 0 ||
				yRelative < 0 || yRelative >= layer->yLength ||
				xRelative < 0 || xRelative >= layer->xLength)
			continue;
		Sprite *sprite = &(layer->sprite[yRelative][xRelative]);
		auto icons = (std::vector<std::array<char, 3>> *) sprite->icon;
		if (icons->empty())
			continue;
		mvaddnstr(y, x * 2, icons->back().begin(), 2);
		drawn = 1;
		break;
	}
	if (!drawn) {
		mvaddnstr(y, x * 2, "  ", 2);
	}
}
