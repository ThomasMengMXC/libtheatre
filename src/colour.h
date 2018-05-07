#ifndef COLOUR_H
#define COLOUR_H
// AUSSIE AUSSIE AUSSIE

#include <stdint.h>

typedef struct Colour {
	uint8_t r, b, g, a;
} Colour;

uint8_t rgb_to_term8(uint8_t r, uint8_t g, uint8_t b);
uint8_t rgb_to_term16(uint8_t r, uint8_t g, uint8_t b);
uint8_t magnitude8(uint8_t colour);

uint8_t rgb_to_term256(uint8_t r, uint8_t g, uint8_t b);
uint8_t magnitude256(uint8_t colour);

uint8_t mix_colours(Colour *colour, uint8_t depth);

#endif
