// AUSSIE AUSSIE AUSSIE
#include <stdlib.h>
#include <stdint.h>
#include "colour.h"

uint8_t rgb_to_term8(uint8_t r, uint8_t g, uint8_t b) {
	if (r == 192 || g == 192 || b == 192) return 7;
	if (r == 128 && g == 128 && b == 128) return 8;
	return magnitude8(r) + magnitude8(g) * 2 + magnitude8(b) * 4;
}

uint8_t rgb_to_term16(uint8_t r, uint8_t g, uint8_t b) {
	if (r == 255 || g == 255 || b == 255) {
		return (r == 255) + (g == 255) * 2 + (b == 255) * 4 + 8;
	}
	if (r == 192 || g == 192 || b == 192) return 7;
	if (r == 128 && g == 128 && b == 128) return 8;
	return magnitude8(r) + magnitude8(g) * 2 + magnitude8(b) * 4;
}

uint8_t magnitude8(uint8_t colour) {
	if (colour >= 128) return 1;
	if (colour >= 0) return 0;
}

uint8_t rgb_to_term256(uint8_t r, uint8_t g, uint8_t b) {
	return 16 + 36 * magnitude256(r) + 6 * magnitude256(g) + magnitude256(b);
}

uint8_t magnitude256(uint8_t colour) {
	if (colour == 255) return 5;
	if (colour >= 215) return 4;
	if (colour >= 175) return 3;
	if (colour >= 135) return 2;
	if (colour >= 95) return 1;
	if (colour >= 0) return 0;
}
