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
	POINT getSize();
};

class Circle : public Drawable {
private:
	long float _radius; PixelBit _color;
public:
	Circle(long float radius, PixelBit color);
	PixelBit getPixel(POINT pos) override;
};

enum errorcode {
	OK,
	FileReadError,
	libStructErr,
	miscError
};

class SEImage : public Drawable {
private:
	uint32_t* data = nullptr;
	size_t s = 0;
public:
	POINT getSize();
	errorcode loadFile(const char* filename);
	PixelBit getPixel(POINT pos) override;
	~SEImage();
};

class Mask : public Drawable {
private:
	Drawable *up = nullptr, *mask = nullptr;
	POINT upTransform;
public:
	Mask(Drawable* up, Drawable* mask, const POINT& upTransform);
	PixelBit getPixel(POINT pos) override;
};
