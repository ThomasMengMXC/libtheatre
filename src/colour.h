#ifndef COLOUR_H
#define COLOUR_H

#include <stdint.h>

typedef enum ColourType {
	REGULAR,
	BRIGHT,
	DETAILED,
	GREY,
} ColourType;

uint8_t rgb_to_term(uint8_t r, uint8_t g, uint8_t b);
uint8_t magnitude(uint8_t colour);

#endif
