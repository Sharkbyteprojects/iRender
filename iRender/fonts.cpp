#include "origin.h"
#include "fonts.hpp"

FontBuilder::FontBuilder()
{
	if (FT_Init_FreeType(&library)) err = true;
	else lOk = true;
}

bool FontBuilder::getErr()
{
	bool e = err;
	err = false;
	return e || !lOk;
}

FontBuilder::~FontBuilder()
{
	for (size_t i = 0; i < tfd.size(); i++) {
		delete (tfd[i]);
	}
	if (lOk) FT_Done_FreeType(library);
}

theFontDefinition* FontBuilder::fontMaker(const char* filename, FT_UInt size, FT_UInt lineSpacing, FT_Long face_index)
{
	theFontDefinition* fd = new theFontDefinition(&library, filename, size, lineSpacing, face_index);
	if (fd == nullptr)
		return nullptr;
	if (fd->getError()) {
		delete fd;
		return nullptr;
	}
	tfd.push_back(fd);
	return fd;
}

FT_Library* FontBuilder::getRaw()
{
	return &library;
}
