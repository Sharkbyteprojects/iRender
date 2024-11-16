#include "tfd.hpp"
#define onlyHelper
#include "helps.hpp"

theFontDefinition::theFontDefinition(FT_Library* ftlib, const char* filename, FT_UInt size, FT_UInt lineSpacing, FT_Long face_index) :size(size), lineSpacing(lineSpacing)
{
	if (FT_New_Face(*ftlib, filename, 0, &font))
		error = true;
	else
		loerror = false;

	memset(&color, 0xff, sizeof(PixelBit));
}

void theFontDefinition::changeSize(FT_UInt si)
{
	size = si;
}

void theFontDefinition::changeLineSpacing(FT_UInt lineSpacing)
{
	this->lineSpacing = lineSpacing;
}

void theFontDefinition::setColor(const PixelBit& pbit)
{
	color.RGBA = pbit.RGBA;
}

void theFontDefinition::_draw(PixelBit* bitmap, std::string textToPrint, sizeDef bitmapSize, size_t penx, size_t peny)
{
	size_t pen_y = peny + size, pen_x = penx;
	FT_Set_Pixel_Sizes(font, 0, size);

	for (size_t i = 0; i < textToPrint.size(); i++) {
		const char* p = textToPrint.c_str() + i;
		if (*p == '\n') {
			pen_y += size + lineSpacing;
			pen_x = penx;
			continue;
		}
		if (FT_Load_Char(font, *p, FT_LOAD_RENDER)) {
			error = true;
			continue;
		}

		PixelBit builder;
		FT_GlyphSlot g = font->glyph;
		for (int y = 0; y < g->bitmap.rows; y++) {
			for (int x = 0; x < g->bitmap.width; x++) {
				int index = (pen_y + y - g->bitmap_top) * bitmapSize.width + (pen_x + x + g->bitmap_left);
				if (index >= 0 && index < bitmapSize.bitSize) {
					setFont(&builder, g->bitmap.buffer[y * g->bitmap.width + x], color);

					blendPixel(&bitmap[index], builder);
				}
			}
		}

		pen_x += g->advance.x >> 6;
	}
}

bool theFontDefinition::getError()
{
	bool e = error;
	error = false;
	return e || loerror;
}

theFontDefinition::~theFontDefinition()
{
	if (!loerror)
		FT_Done_Face(font);
}
