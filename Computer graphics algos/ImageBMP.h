#pragma once

#include"stb_image_write.h"

#include<algorithm>
#include<array>
#include<cassert>
#include<filesystem> 
#include<fstream> 
#include<iomanip> 
#include<iostream>
#include<map> 
#include<string>
#include<unordered_map>
#include <vector>

#include"Utils.h"
#include"Vec2.h"


#include <SDL3/SDL_stdinc.h> // for Uint8

#ifdef __cplusplus
#if __cplusplus >= 201703L
using std::cout, std::ifstream, std::ofstream, std::vector, std::string, std::ios, std::ostream, std::array;
using std::swap, std::map, std::pair, std::setw, std::left, std::to_string;
using std::unordered_map;
#endif
#endif

using std::map; 
using std::vector; 
using std::array; 
using std::ofstream; 
using std::ifstream; 
using std::string; 

class FileHeader
{
	/*will make PRIVATE all of the bmp fields that (probably) never change
	- ex: 1st two bytes should always be B and M*/

	array<char, 2> filetype = { 'B', 'M' }; //indices 0 and 1  
	//const char* filetype = "BM"; //indices 0 and 1  -> problematic type!

	unsigned int reserved1And2 = 0x00'00'00'00; //(unclear meaning) indices 6 - 9 
	unsigned int indexOfPixelData = 0x00'00'00'36; //indices 10 - 13

public:
	unsigned int fileSize{}; //indices 2 - 5 (size is in bytes)
	//should perhaps be set by the ImageBMP class //(since ImageBMP will have info on pixel data and infoheader) 

	FileHeader() = default;

	friend class ImageBMP;

};

class InfoHeader
{
	unsigned int infoHeaderSize = 0x00'00'00'28; //indices 14 - 17, in bytes
	short planes = 0x00'01; //indices 26 - 27 ["always" 1 (meaning unclear)] 

	short bitsPerPixel = 0x00'20; //CAREFUL! not always 32! -> indices 28 - 29 (32 bits - 24 for G,B,R, and 8 for Alpha)
	unsigned int compressionMethod = 0x00'00'00'00; //indices 30 - 33
	unsigned int sizeOfPixelData{};// = imageWidth * imageHeight * (bitsPerPixel / 8); //indices 34 - 37
	std::vector<int> remainingHeaderFields =
	{
		0x00'00'00'00,//xPixelsPerMeter
		0x00'00'00'00,//yPixelsPerMeter
		0x00'00'00'00,//color indexing thing
		0x00'00'00'00//"important" color count 
	};

public:
	unsigned int imageWidth = 0; //indices 18 - 21
	unsigned int imageHeight = 0; //indices 22 - 25

	InfoHeader() = default;

	unsigned int getInfoHeaderSize() const;
	unsigned int getSizeOfPixelData() const;

	friend class ImageBMP;
};

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

	MediumGreen =	0xFF'00'CC'00,               
	DarkerGreen =	0xFF'00'99'00,                
	LightGreen =	0xFF'33'FF'33,                
	Pastel =		0xFF'66'FF'66,                
	Forest =		0xFF'00'66'33,                
	Midtone =		0xFF'00'AA'55,                
	Spring =		0xFF'11'FF'77,                
	TealGreen =		0xFF'44'CC'88,               
	PaleGreen =		0xFF'88'FF'AA,

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
	/*NOTE that I include 'a' but alpha is not generally supported by BMP
	* I used alpha mainly to simplify "4 byte padding logic" 
	*/
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


	friend class ImageBMP; 
};

class PixelData
{
private:
	/*NOTA BENE! usage is pixelMatrix[ROW][COLUMN]!*/
	std::vector<vector<Color>> pixelMatrix;
public:

	PixelData() = default;

	friend class ImageBMP; //ImageBMP is composed of PixelData
};

class ImageBMP
{
	void readFileHeaderFromFile(ifstream& fin);
	void readInfoHeaderFromFile(ifstream& fin);
	void readPixelDataFromFile(ifstream& fin);
	FileHeader fileHeader;
	InfoHeader infoHeader;
	PixelData pixelData;

public:
	ImageBMP() = default;

	ImageBMP(unsigned int imageWidth, unsigned int imageHeight, const Color& fillColor);

	ImageBMP(const string& filepath);

	ImageBMP(const std::vector<Vec2>& points, Color bgrdColor = ColorEnum::Black);

	/*OBSERVE the "switched" x and y coordinates in the update to `pixelMatrix` in this function definition!*/
	void fillPixelMatrix(const std::unordered_map<Vec2, Color>& pointsToColors);
	
	/*An overload that sets default color of all pixels (which can be changed, if desired)*/
	void fillPixelMatrix(const std::vector<Vec2>& points, Color defaultColor = ColorEnum::Magenta);

	/*BMP is uncompressed - filesize may be LARGE*/
	void saveAsBMP(std::string filename);

	/*requires stb_image_write.h from: https://github.com/nothings/stb
	* PNG file sizes can potentially be much (100x) smaller than BMP
	* INTERESTINGLY, PNG compression is LOSSLESS
	*/
	void saveAsPNG(const std::string& PNGfilename);

private: 
	/*Anticipate this only being called by `ImageBMP(const string& filepath)`*/
	void readImageBMP(string inputFilename); };






#pragma region auxillary functions 
//"auxillary" method: 
vector<vector<char>> rotateMatrixClockwise
(vector<vector<char>>& originalMatrix, int originalNumberOfRows, int originalNumberOfCols);

vector<vector<int>> rotateIntMatrixClockwise(vector<vector<int>>& originalMatrix, int originalNumberOfRows, int originalNumberOfCols);

/*NOTE: this function requires C++17!
And caution: potentially returning "large" amount of data*/
vector<ImageBMP> getAllImagesInFolder(string folderName);
