#pragma once

#include <SDL3/SDL_stdinc.h> // for Uint8
#include<array>
#include <vector>


/*NOTE: little-endian ARGB order is used here*/
enum class ColorEnum : unsigned int
{
	//A, R, G, B
	Black = 0xFF'00'00'00,
	White = 0xFF'FF'FF'FF,

	Red = 0xFF'FF'00'00,
	Green = 0xFF'00'FF'00,
	Blue = 0xFF'00'00'FF,

	Yellow = 0xFF'FF'FF'00,
	Cyan = 0xFF'00'FF'FF,
	Magenta = 0xFF'FF'00'FF,

	Orange = 0xFF'FF'80'00,
	Purple = 0xFF'80'00'80,

	Lime = 0xFF'BFFF'00,
	Teal = 0xFF'00'80'80,

	Pink = 0xFF'FF'66'CC,
	Brown = 0xFF'80'40'00,

	Gray = 0xFF'80'80'80,
	Olive = 0xFF'80'80'00,

	Indigo = 0xFF'4B'00'82,
	Gold = 0xFF'FF'D7'00,

	MediumGreen = 0xFF'00'CC'00,
	DarkerGreen = 0xFF'00'99'00,
	LightGreen = 0xFF'33'FF'33,
	Pastel = 0xFF'66'FF'66,
	Forest = 0xFF'00'66'33,
	Midtone = 0xFF'00'AA'55,
	Spring = 0xFF'11'FF'77,
	TealGreen = 0xFF'44'CC'88,
	PaleGreen = 0xFF'88'FF'AA,

	//(not 50, but 10) shades of gray:
	G1 = 0xFF111111, // dark
	G2 = 0xFF222222,
	G3 = 0xFF333333,
	G4 = 0xFF444444,
	G5 = 0xFF555555,
	G6 = 0xFF666666,
	G7 = 0xFF777777,
	G8 = 0xFF888888,
	G9 = 0xFF999999,
	G10 = 0xFFAAAAAA  // light
};



class Color
{
	//should be unsigned because 1) no "negative" colors and 2) having alpha = 255 (FF) is desirable
	unsigned int bgra = 0x00'00'00'00;
	unsigned int convertToUnsignedInt();
	/*NOTE that I include 'a' but alpha is not generally supported by BMP - NOTE: I used alpha mainly to simplify "4 byte padding logic"*/
	Color(unsigned int b, unsigned int g, unsigned int r, unsigned int a); //just in case, I suppose...
	// Individual getters (called by Color::getRGB)
	unsigned int getB() const;
	unsigned int getG() const;
	unsigned int getR() const;

public:
	Color() = default;
	Color(unsigned int r, unsigned int g, unsigned int b);
	Color(ColorEnum colorEnum);

	static std::vector<ColorEnum> getBroadColorPalette();
	static std::vector<ColorEnum> getGreenColorPalette();
	static std::vector<ColorEnum> getGrayColorPalette();

	std::array<unsigned int, 3> getRGB() const;

	void setB(unsigned int b);
	void setG(unsigned int g);
	void setR(unsigned int r);

	/*Methods for interfacing with SDL:*/

	Uint8 getR8() const { return static_cast<Uint8>(getR()); }
	Uint8 getG8() const { return static_cast<Uint8>(getG()); }
	Uint8 getB8() const { return static_cast<Uint8>(getB()); }

	void getRGB8(Uint8& r, Uint8& g, Uint8& b) const {
		r = static_cast<Uint8>(getR());
		g = static_cast<Uint8>(getG());
		b = static_cast<Uint8>(getB());
	}

};


