#pragma once
#include "origin.h"

void save_png(const char* filename, PixelBit* bitmap, size_t biSi, size_t width, size_t height);
void blendPixel(PixelBit* back, const PixelBit& front);
void setFont(PixelBit* pbit, uint8_t fontBit, const PixelBit& color);

#ifndef onlyHelper
#include "tfd.hpp"
class ImageEne {
private:
	PixelBit* bitmap = nullptr;

protected:
	bool error = false, syserr = false;
	size_t width, height, bitSize, bsize;

public:
	ImageEne(size_t width, size_t height);
	void clear(uint8_t R = 0, uint8_t G = 0, uint8_t B = 0, uint8_t A = 0);
	void savePng(const char* filename);
	PixelBit* bitmapR();
	const bool errorG();
	~ImageEne();
	void overlayPixel(const PixelBit& pbit, size_t x, size_t y);

	void drawText(theFontDefinition* font, std::string textToPrint, size_t xStartPosition, size_t yStartPosition);

	sizeDef GetbitmapSize();
};
#endif
