#include "helps.hpp"
#include <png.h>
#include <vector>
#include <iostream>

// Function to blend two pixel bytes using alpha
uint8_t blendPixel(uint8_t background, uint8_t foreground, uint8_t alpha) {
	float alphaF = alpha / 255.0f; // Convert alpha to a float between 0 and 1
	return static_cast<uint8_t>(background * (1 - alphaF) + foreground * alphaF);
}

void blendPixel(PixelBit* back, const PixelBit& front) {
	for (uint8_t i = 0; i < pixelBytes - 1; i++) {
		back->RGBA_C[i] = blendPixel(back->RGBA_C[i], front.RGBA_C[i], front.A);
	}
}

void setFont(PixelBit* pbit, uint8_t fontBit, const PixelBit& color) {
	float mbit = 0;
	for (uint8_t i = 0; i < pixelBytes - 1; i++) {
		mbit = color.RGBA_C[i] / 255.0f;
		pbit->RGBA_C[i] = static_cast<uint8_t>(fontBit * (1 - mbit));
	}
	mbit = color.A / 255.0f;
	pbit->A = fontBit * mbit;
}

// Function to save the bitmap as a PNG file
void save_png(const char* filename, PixelBit* bitmap, size_t biSi, size_t width, size_t height) {
#pragma warning(suppress : 4996)
	FILE* fp = fopen(filename, "wb");
	if (!fp) {
		std::cerr << "Could not open file for writing" << std::endl;
		return;
	}

	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) {
		std::cerr << "Could not create PNG write structure" << std::endl;
		fclose(fp);
		return;
	}

	png_infop info = png_create_info_struct(png);
	if (!info) {
		std::cerr << "Could not create PNG info structure" << std::endl;
		png_destroy_write_struct(&png, NULL);
		fclose(fp);
		return;
	}

	if (setjmp(png_jmpbuf(png))) {
		std::cerr << "Error during PNG creation" << std::endl;
		png_destroy_write_struct(&png, &info);
		fclose(fp);
		return;
	}

	png_init_io(png, fp);
	png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png, info);

	std::vector<png_bytep> rows(height);
	size_t pos = 0;
	for (size_t y = 0; y < height; y++) {
		pos = y * width;
		if (pos > biSi) break;
		rows[y] = (unsigned char*)&bitmap[pos].RGBA;
	}

	png_write_image(png, rows.data());
	png_write_end(png, NULL);
	png_destroy_write_struct(&png, &info);
	fclose(fp);
}

ImageEne::ImageEne(size_t width, size_t height) :width(width), height(height)
{
	bitSize = width * height;
	bsize = (bitSize + 1) * sizeof(PixelBit);

	bitmap = (PixelBit*)malloc(bsize);
	if (bitmap == nullptr) {
		std::cerr << "Can't alloc space!" << std::endl;
		error = true;
		syserr = true;
	}
}

void ImageEne::clear(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
{
	for (size_t i = 0; i < bitSize; i++) {
		bitmap[i].R = R;
		bitmap[i].G = G;
		bitmap[i].B = B;
		bitmap[i].A = A;
	}
}

void ImageEne::savePng(const char* filename)
{
	save_png(filename, bitmap, bitSize, width, height);
}

PixelBit* ImageEne::bitmapR()
{
	return bitmap;
}

const bool ImageEne::errorG()
{
	bool err = error;
	error = false;
	return err || syserr;
}

ImageEne::~ImageEne()
{
	if (bitmap != nullptr)
		free(bitmap);
	bitmap = nullptr;
}

void ImageEne::overlayPixel(const PixelBit& pbit, size_t x, size_t y)
{
	if (y > height || x > width) return;
	size_t index = x + y * width;
	if (index >= bitSize) {
		error = true;
		return;
	}

	blendPixel(&bitmap[index], pbit);
}


void ImageEne::drawText(theFontDefinition* font, std::string textToPrint, size_t xStartPosition, size_t yStartPosition)
{
	if (font->getError()) {
		error = true;
		return;
	}

	font->_draw(bitmap, textToPrint, this->GetbitmapSize(), xStartPosition, yStartPosition);
	error = font->getError();
}

sizeDef ImageEne::GetbitmapSize()
{
	sizeDef defined;
	defined.bitSize = bitSize;
	defined.bsize = bsize;
	defined.width = width;
	defined.height = height;
	return defined;
}
