#ifndef TSHTFD
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <vector>
#include "origin.h"

class theFontDefinition {//Struct gets initialized by a FontBuilder::fontMaker
private:
	FT_Face font;
	FT_UInt size = 0, lineSpacing = 0;
	PixelBit color;

	bool error = false,
		loerror = true;
public:
	theFontDefinition(FT_Library* ftlib, const char* filename, FT_UInt size, FT_UInt lineSpacing, FT_Long face_index = 0);
	void changeSize(FT_UInt si);
	void changeLineSpacing(FT_UInt lineSpacing);
	void setColor(const PixelBit& pbit);

	void _draw(PixelBit* bitmap, std::string textToPrint, sizeDef bitmapSize, size_t penx, size_t peny);

	bool getError();
	~theFontDefinition();
};
#define TSHTFD
#endif
