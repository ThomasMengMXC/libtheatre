// AUSSIE AUSSIE AUSSIE

#include <stdlib.h>
#include <stdint.h>
#include "colour.h"

uint8_t rgb_to_term(uint8_t r, uint8_t g, uint8_t b) {
	return 16 + 36 * magnitude(r) + 6 * magnitude(g) + magnitude(b);
}

uint8_t magnitude(uint8_t colour) {
	if (colour == 0) {
		return 0;
	} else if (colour <= 95) {
		return 1;
	} else if (colour <= 135) {
		return 2;
	} else if (colour <= 175) {
		return 3;
	} else if (colour <= 215) {
		return 4;
	} else if (colour <= 255) {
		return 5;
	}
}
