#ifndef COLOUR_H
#define COLOUR_H

#include <stdint.h>

typedef enum ColourType {
	REGULAR,
	BRIGHT,
	DETAILED,
	GREY,
} ColourType;

uint8_t rgb_to_term8(uint8_t r, uint8_t g, uint8_t b);
uint8_t rgb_to_term16(uint8_t r, uint8_t g, uint8_t b);
uint8_t magnitude8(uint8_t colour);

uint8_t rgb_to_term256(uint8_t r, uint8_t g, uint8_t b);
uint8_t magnitude256(uint8_t colour);

#endif
