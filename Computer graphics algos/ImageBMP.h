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

#include"Vec2.h"


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

/*NOTE: little-endian BGRA order is used here*/
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

	//chessboard-specific colors: 
	DarkSquareColor = 0xFF'BA'61'34,
	LightSquareColor = 0xFF'EF'D7'B5,
	BoardBorder = 0xFF'6C'1E'1C,

	//using some images from this page: https://commons.wikimedia.org/wiki/Category:Chess_bitmap_pieces
	WKnightBgrdColor = 0xFF'FF'FF'CB,

	RedBgrd = 0xFF'ED'1C'24
	//CopyWKnightBgrdColor = 0x00'00'00'00

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

	std::array<unsigned int, 3> getRGB() const;
	
	void setB(unsigned int b);
	void setG(unsigned int g);
	void setR(unsigned int r);

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

	/*BMP is uncompressed - filesize may be LARGE*/
	void saveAsBMP(std::string filename);

	/*OBSERVE the "switched" x and y coordinates in the update to `pixelMatrix` in this function definition!*/
	void fillPixelMatrix(const std::unordered_map<Vec2, Color>& pixelsToColors);

	/*requires stb_image_write.h from: https://github.com/nothings/stb
	* PNG file sizes can potentially be much (100x) smaller than BMP
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
