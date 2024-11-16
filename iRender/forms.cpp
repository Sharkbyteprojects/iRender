#include "forms.hpp"
#include <cmath>
#include <png.h>

void Drawable::createPlace(POINT size)
{
	memcpy(&this->size, &size, sizeof(POINT));
}

bool Drawable::inbound(const POINT& poe)
{
	return size.x > poe.x && size.y > poe.y;
}

PixelBit Drawable::getPixel(POINT pos)
{
	return { 0,0,0,0 };
}


void Drawable::drawOnImage(std::shared_ptr<ImageEne> i, POINT position)
{
	POINT pos{ 0,0 };
	for (pos.y = 0; pos.y < size.y; pos.y++) {
		for (pos.x = 0; pos.x < size.x; pos.x++) {
			i->overlayPixel(getPixel(pos), position.x + pos.x, position.y + pos.y);
		}
	}
}

POINT Drawable::getSize()
{
	return size;
}

Circle::Circle(long float radius, PixelBit color) :_radius(radius), _color(color)
{
	auto hw = ((unsigned long long)radius) * 2;
	createPlace({ hw, hw });
}

PixelBit Circle::getPixel(POINT pos)
{
	if (inbound(pos)) {
		auto pythogoras = sqrt(pow(_radius - pos.y, 2) + pow(_radius - pos.x, 2));
		if (pythogoras < _radius)
			return _color;
	}
	return { 0,0,0,0 };
}

POINT SEImage::getSize()
{
	return size;
}

errorcode SEImage::loadFile(const char* filename)
{
	if (data != nullptr) delete[] data;
#pragma warning(suppress : 4996)
	FILE* fp = fopen(filename, "rb");
	if (!fp)
		return FileReadError;
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) {
		fclose(fp);
		return libStructErr;
	}
	png_infop info = png_create_info_struct(png);
	if (!info) {
		png_destroy_read_struct(&png, NULL, NULL);
		fclose(fp);
		return libStructErr;
	}
	if (setjmp(png_jmpbuf(png))) {
		png_destroy_read_struct(&png, &info, NULL); fclose(fp); return miscError;
	}
	png_init_io(png, fp);
	png_read_info(png, info);
	auto width = png_get_image_width(png, info);
	auto height = png_get_image_height(png, info);

	createPlace({ width, height });

	png_byte color_type = png_get_color_type(png, info);
	png_byte bit_depth = png_get_bit_depth(png, info);
	if (bit_depth == 16) png_set_strip_16(png);
	if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
	if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
	if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE) png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
	if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png);
	png_read_update_info(png, info);

	s = width * height;
	data = new uint32_t[s + 1];

	std::vector<png_bytep> rows(height);
	for (size_t i = 0; i < height; i++) {
		rows[i] = (uint8_t*)(&data[(width)*i]);
	}
	png_read_image(png, rows.data());

	fclose(fp);
	png_destroy_read_struct(&png, &info, NULL);
	return OK;
}

PixelBit SEImage::getPixel(POINT pos)
{
	if (!inbound(pos)) return { 0,0,0,0 };
	size_t readIt = pos.x + (pos.y * (size.y - 1));
	if (readIt > s) return { 0,0,0,0 };

	PixelBit pbit;
	pbit.RGBA = data[readIt];
	return pbit;
}

SEImage::~SEImage()
{
	if (data != nullptr)
		delete[] data;
}

Mask::Mask(Drawable* upper, Drawable* mask, const POINT& upTransform)
	:up(upper), mask(mask), upTransform(upTransform)
{
	createPlace(mask->getSize());
}

PixelBit Mask::getPixel(POINT pos)
{
	auto maskBit = mask->getPixel(pos);
	auto imageBit = up->getPixel({ pos.x + upTransform.x, pos.y + upTransform.y });
	imageBit.A = maskBit.A;
	return imageBit;
}
