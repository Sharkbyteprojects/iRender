#pragma once
#include <cstdint>

#define pixelBytes 4
union PixelBit {
	struct {
		uint8_t R;
		uint8_t G;
		uint8_t B;
		uint8_t A;
	};
	uint32_t RGBA;
	uint8_t RGBA_C[pixelBytes];
};

struct sizeDef {
	size_t width, height, bitSize, bsize;
};

