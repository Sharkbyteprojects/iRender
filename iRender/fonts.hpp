#pragma once
#include "tfd.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <vector>

class FontBuilder {
private:
	FT_Library library;
	bool err = false;
	bool lOk = false;
	std::vector<theFontDefinition*> tfd;
public:
	FontBuilder();
	bool getErr();
	~FontBuilder();

	theFontDefinition* fontMaker(const char* filename, FT_UInt size, FT_UInt lineSpacing, FT_Long face_index = 0);

	FT_Library* getRaw();
};
