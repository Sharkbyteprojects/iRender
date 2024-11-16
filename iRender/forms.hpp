#include "helps.hpp"
#include <memory>

struct POINT {
	size_t x, y;
};

class Drawable {
protected:
	POINT size;
	void createPlace(POINT size);
	bool inbound(const POINT& poe);
public:
	virtual PixelBit getPixel(POINT pos);
	void drawOnImage(std::shared_ptr<ImageEne> i, POINT position);
};

class Circle : public Drawable {
private:
	long float _radius; PixelBit _color;
public:
	Circle(long float radius, PixelBit color);
	PixelBit getPixel(POINT pos) override;
};
