#include "forms.hpp"
#include <cmath>

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
