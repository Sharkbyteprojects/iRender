#include <iostream>
#include "helps.hpp"
#include "fonts.hpp"
#include "forms.hpp"
#include <memory>

int main() {
	std::shared_ptr<FontBuilder> theFont = std::make_shared<FontBuilder>();
	if (theFont->getErr()) {
		std::cerr << "Could not initialize FreeType library" << std::endl;
		return -1;
	}

	theFontDefinition* myFont = theFont->fontMaker("assets/Roboto-Regular.ttf", 20, 2);
	if (myFont == nullptr) {
		std::cerr << "Could not load font" << std::endl;
		return -1;
	}

	std::shared_ptr<ImageEne> bitmaps = std::make_shared<ImageEne>(800, 600);//Args: width, height
	if (bitmaps->errorG()) {
		std::cerr << "Can't alloc space!" << std::endl;
		return -1;
	}

	bitmaps->clear(0x36, 0x39, 0x3F, 0xff);

	bitmaps->overlayPixel({ 0, 0, 0, 255 }, 800 / 2, 600 / 2);

	Circle circle1(80, { 255, 255, 0, 255 });
	circle1.drawOnImage(bitmaps, { 800 / 2 - 80, 600 / 2 - 80 });

	{
		SEImage myTest;
		auto r = myTest.loadFile("assets/gravatar-user-420.png");
		if (r == errorcode::OK) {
			auto sizeOfTestImg = myTest.getSize();
			myTest.drawOnImage(bitmaps, { 800 / 2 - (sizeOfTestImg.x / 2), 600 / 2 - (sizeOfTestImg.y / 2) });
			{
				Circle mb(sizeOfTestImg.x/2, { 0,0,0,0xff });
				Mask m((Drawable*) &myTest, (Drawable*)&mb, {0,0});
				m.drawOnImage(bitmaps, { 20, 30 });
			};
		}
	};


	myFont->setColor({ 0x43, 0x94, 0xfb, 0xff });

	bitmaps->drawText(myFont, "Hello Test!\nMy Test!!", 136, 60);
	if (bitmaps->errorG()) {
		std::cerr << "Could not load some character" << std::endl;
	}

	bitmaps->savePng("output.png");

	std::cout << "Text rendered and saved as output.png successfully" << std::endl;
	return 0;
}
