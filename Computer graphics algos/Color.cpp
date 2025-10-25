#include "Color.h"

#pragma region Color


Color::Color(unsigned int r, unsigned int g, unsigned int b)
{
	//bgra = (b << 0) | (g << 8) | (r << 16) | (0xFF << 24); // Set alpha to 255

	bgra = (b << 0) | (g << 8) | (r << 16) | (0xFF << 24); // alpha = 255
}

Color::Color(unsigned int b, unsigned int g, unsigned int r, unsigned int a)
{
	bgra =
		(
			b << 0 |
			g << 8 |
			r << 16 |
			a << 24
			);
}

Color::Color(ColorEnum colorEnum)
	:bgra((unsigned int)colorEnum) //note the typecast
{

}

std::array<unsigned int, 3> Color::getRGB() const
{
	std::array<unsigned int, 3> rgb =
	{
		getR(),
		getG(),
		getB()
	};

	return rgb;
}

unsigned int Color::convertToUnsignedInt()
{
	return bgra;
}

unsigned int Color::getB() const
{
	return (bgra >> 0) & 0xFF;
}

unsigned int Color::getG() const
{
	return (bgra >> 8) & 0xFF;
}

unsigned int Color::getR() const
{
	return (bgra >> 16) & 0xFF;
}

void Color::setB(unsigned int b)
{
	bgra = (bgra & 0xFFFFFF00) | (b & 0xFF);
}

void Color::setG(unsigned int g)
{
	bgra = (bgra & 0xFFFF00FF) | ((g & 0xFF) << 8);
}

void Color::setR(unsigned int r)
{
	bgra = (bgra & 0xFF00FFFF) | ((r & 0xFF) << 16);
}

std::vector<ColorEnum> Color::getBroadColorPalette()
{
	std::vector<ColorEnum> palette = {
		ColorEnum::Red,
		ColorEnum::Green,
		ColorEnum::Blue,
		ColorEnum::Cyan,
		ColorEnum::Magenta,
		ColorEnum::Yellow,
		ColorEnum::Orange,
		ColorEnum::Purple,
		ColorEnum::Lime,
		ColorEnum::Teal
	};

	return palette;
}
std::vector<ColorEnum> Color::getGreenColorPalette()
{
	std::vector<ColorEnum> greenPalette =
	{
		ColorEnum::Green,
		ColorEnum::MediumGreen,
		ColorEnum::DarkerGreen,
		ColorEnum::LightGreen,
		ColorEnum::Pastel,
		ColorEnum::Forest,
		ColorEnum::Midtone,
		ColorEnum::Spring,
		ColorEnum::TealGreen,
		ColorEnum::PaleGreen
	};

	return greenPalette;
}

std::vector<ColorEnum> Color::getGrayColorPalette()
{
	std::vector<ColorEnum> grayPalette =
	{
		ColorEnum::G1,
		ColorEnum::G2,
		ColorEnum::G3,
		ColorEnum::G4,
		ColorEnum::G5,
		ColorEnum::G6,
		ColorEnum::G7,
		ColorEnum::G8,
		ColorEnum::G9,
		ColorEnum::G10,
	};

	return grayPalette;
}


#pragma endregion 