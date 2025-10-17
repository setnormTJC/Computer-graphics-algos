#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ImageBMP.h"



#pragma region ImageBMP
void ImageBMP::saveAsBMP(std::string filename)
{
	ofstream fout{ filename, std::ios::binary };

	//first comes the 14-byte file header: 
	fout.write(reinterpret_cast<const char*>(fileHeader.filetype.data()), 2); //no sizeof here, since filetype is a pointer
	fout.write(reinterpret_cast<const char*>(&fileHeader.fileSize), sizeof(fileHeader.fileSize));
	fout.write(reinterpret_cast<const char*>(&fileHeader.reserved1And2), sizeof(fileHeader.reserved1And2));
	fout.write(reinterpret_cast<const char*>(&fileHeader.indexOfPixelData), sizeof(fileHeader.indexOfPixelData));

	//next, the 40-byte info header: 
	fout.write(reinterpret_cast<const char*>(&infoHeader.infoHeaderSize), sizeof(infoHeader.infoHeaderSize));
	fout.write(reinterpret_cast<const char*>(&infoHeader.imageWidth), sizeof(infoHeader.imageWidth));
	fout.write(reinterpret_cast<const char*>(&infoHeader.imageHeight), sizeof(infoHeader.imageHeight));
	fout.write(reinterpret_cast<const char*>(&infoHeader.planes), sizeof(infoHeader.planes));
	fout.write(reinterpret_cast<const char*>(&infoHeader.bitsPerPixel), sizeof(infoHeader.bitsPerPixel));
	fout.write(reinterpret_cast<const char*>(&infoHeader.compressionMethod), sizeof(infoHeader.compressionMethod));
	fout.write(reinterpret_cast<const char*>(&infoHeader.sizeOfPixelData), sizeof(infoHeader.sizeOfPixelData));

	for (const auto& remainingHeaderField : infoHeader.remainingHeaderFields)
	{
		fout.write(reinterpret_cast<const char*>(&remainingHeaderField), sizeof(remainingHeaderField));
	}

	// Calculate the number of bytes per pixel based on the bit depth
	int bytesPerPixel = infoHeader.bitsPerPixel / 8;

	// Calculate the padding bytes per row (each row is padded to a multiple of 4 bytes)
	int paddingBytes = (4 - (infoHeader.imageWidth * bytesPerPixel) % 4) % 4;

	//now the pixel data: 
	for (unsigned int row = 0; row < infoHeader.imageHeight; ++row)
	{
		for (unsigned int col = 0; col < infoHeader.imageWidth; ++col)
		{
			if (infoHeader.bitsPerPixel == 32)
			{


				unsigned int colorOfCurrentPixel = pixelData.pixelMatrix.at(row).at(col).convertToUnsignedInt();

				fout.write(reinterpret_cast<const char*>(&colorOfCurrentPixel), sizeof(colorOfCurrentPixel));
			}

			else if (infoHeader.bitsPerPixel == 24)
			{
				unsigned int colorOfCurrentPixel = pixelData.pixelMatrix.at(row).at(col).convertToUnsignedInt();
				char rgb[3] = {
					static_cast<char>((colorOfCurrentPixel >> 0) & 0xFF),
					static_cast<char>((colorOfCurrentPixel >> 8) & 0xFF),
					static_cast<char>((colorOfCurrentPixel >> 16) & 0xFF)
				};
				fout.write(rgb, 3);
			}

			else
			{
				std::cout << "Hey! Neither 32 nor 24 bits per pixel? What is this file?\n";
				std::cin.get();
			}
		}

		// Write the padding bytes at the end of each row
		char padding[3] = { 0, 0, 0 };
		fout.write(padding, paddingBytes);
	}

	fout.close();
}


void ImageBMP::fillPixelMatrix(const std::unordered_map<Vec2, Color>& pixelsToColors)
{
	for (const auto& [pixel, color] : pixelsToColors)
	{
		if (pixel.y < pixelData.pixelMatrix.size() &&
			pixel.x < pixelData.pixelMatrix[0].size())
		{
			pixelData.pixelMatrix[pixel.y][pixel.x] = color;
		}
	}

	//for (const auto& pixel : pixelsToColors.)
}

void ImageBMP::saveAsPNG(const std::string& PNGfilename)
{
	if (PNGfilename.find(".png") == std::string::npos)
	{
		throw std::runtime_error("filename must contain .png if saving as PNG");
	}

	int channels = 3; 
	int height = pixelData.pixelMatrix.size();
	int width = pixelData.pixelMatrix[0].size();

	std::vector<unsigned char> buffer(width * height * 3);
	for (int y = 0; y < height; ++y)
	{
		int srcY = height - 1 - y;  // flip vertically (BMP stores from bottom to top, PNG stores top to bottom)
		for (int x = 0; x < width; ++x)
		{
			unsigned int bgra = pixelData.pixelMatrix[srcY][x].bgra;
			buffer[(y * width + x) * 3 + 0] = (bgra >> 16) & 0xFF; // R
			buffer[(y * width + x) * 3 + 1] = (bgra >> 8) & 0xFF;  // G
			buffer[(y * width + x) * 3 + 2] = bgra & 0xFF;         // B
		}
	}

	stbi_write_png(PNGfilename.c_str(), infoHeader.imageWidth, infoHeader.imageHeight,
		channels, buffer.data(), width*3);
}


ImageBMP::ImageBMP(unsigned int imageWidth, unsigned int imageHeight, const Color& fillColor)
{
	infoHeader.imageWidth = imageWidth;
	infoHeader.imageHeight = imageHeight;
	infoHeader.sizeOfPixelData = imageWidth * imageHeight * (infoHeader.bitsPerPixel / 8);
	//NOTE: fileheader size should always be 14 (I think) 
	fileHeader.fileSize = 14 + infoHeader.getInfoHeaderSize() + infoHeader.sizeOfPixelData;



	//fill pixelData with given fill color:
	for (unsigned int row = 0; row < imageHeight; ++row)
	{
		vector<Color> currentRow;
		for (unsigned int col = 0; col < imageWidth; ++col)
		{
			currentRow.push_back(fillColor);
		}
		pixelData.pixelMatrix.push_back(currentRow);
	}
}

ImageBMP::ImageBMP(const string& filepath)
{
	readImageBMP(filepath);
}

void ImageBMP::readImageBMP(string inputFilename)
{
	ifstream fin{ inputFilename, std::ios::binary };

	if (!fin)
	{
		std::cout << "File " << inputFilename << " not found.\n";
		std::cin.get();
		return;
	}

	//first read the file header info: 
	readFileHeaderFromFile(fin);

	//now read info header: 
	readInfoHeaderFromFile(fin);

	readPixelDataFromFile(fin);

	//pixelData.pixelMatrix; 


	//0xb6
	//0x03'A9'B6

	fin.close();
}



void ImageBMP::readFileHeaderFromFile(ifstream& fin)
{
	char filetype[2];
	fin.read(filetype, 2); //NOTE: fin.GET() appends null terminator! (\0)
	fileHeader.filetype.at(0) = filetype[0];
	fileHeader.filetype.at(1) = filetype[1];

	char filesize[4];
	fin.read(filesize, 4);

	//DETAILED approach without bitshifting and bitwise OR: 
	//auto first = (unsigned char)filesize[0];
	//auto second= (unsigned char)filesize[1];
	//auto third = (unsigned char)filesize[2];
	//auto fourth = (unsigned char)filesize[3];
	//cout << first + (second * pow(2, 8)) + (third*pow(2, 16)) + (fourth*pow(2, 24)) << "\n";

	//equivalently (faster, probably): 
	fileHeader.fileSize =
		(
			(unsigned char)filesize[0] << 0 |
			(unsigned char)filesize[1] << 8 |
			(unsigned char)filesize[2] << 16 |
			(unsigned char)filesize[3] << 24
			);
	char reserved1And2[4];
	fin.read(reserved1And2, 4);

	fileHeader.reserved1And2 = (
		(unsigned char)reserved1And2[0] << 0 |
		(unsigned char)reserved1And2[1] << 8 |
		(unsigned char)reserved1And2[2] << 16 |
		(unsigned char)reserved1And2[3] << 24
		);

	char indexOfPixelData[4];
	fin.read(indexOfPixelData, 4);
	fileHeader.indexOfPixelData = (
		(unsigned char)indexOfPixelData[0] << 0 |
		(unsigned char)indexOfPixelData[1] << 8 |
		(unsigned char)indexOfPixelData[2] << 16 |
		(unsigned char)indexOfPixelData[3] << 24
		);

}

void ImageBMP::readInfoHeaderFromFile(ifstream& fin)
{
	char infoHeaderSize[4];
	fin.read(infoHeaderSize, 4);
	infoHeader.infoHeaderSize =
		(
			(unsigned char)infoHeaderSize[0] << 0 |
			(unsigned char)infoHeaderSize[1] << 8 |
			(unsigned char)infoHeaderSize[2] << 16 |
			(unsigned char)infoHeaderSize[3] << 24
			);

	/*a "safety check" here:*/
	if (infoHeader.infoHeaderSize != 40)
	{
		//cout << "Hey! Listen!\n"; 
		//cout << "Info header size is not 40! - it is " << infoHeader.infoHeaderSize << "\n";
		//std::cin.get(); 
	}


	char imageWidth[4];
	fin.read(imageWidth, 4);
	infoHeader.imageWidth =
		(
			(unsigned char)imageWidth[0] << 0 |
			(unsigned char)imageWidth[1] << 8 |
			(unsigned char)imageWidth[2] << 16 |
			(unsigned char)imageWidth[3] << 24
			);

	char imageHeight[4];
	fin.read(imageHeight, 4);
	infoHeader.imageHeight =
		(
			(unsigned char)imageHeight[0] << 0 |
			(unsigned char)imageHeight[1] << 8 |
			(unsigned char)imageHeight[2] << 16 |
			(unsigned char)imageHeight[3] << 24
			);

	char planes[2];
	fin.read(planes, 2);
	infoHeader.planes =
		(
			(unsigned char)planes[0] << 0 |
			(unsigned char)planes[1] << 8
			);

	char bitsPerPixel[2];
	fin.read(bitsPerPixel, 2);
	infoHeader.bitsPerPixel =
		(
			(unsigned char)bitsPerPixel[0] << 0 |
			(unsigned char)bitsPerPixel[1] << 8
			);

	/*another "safety check" here:*/
	if (infoHeader.bitsPerPixel != 32)
	{
		//cout << "Hey! Listen!\n";
		//cout << "bitsPerPixel is not 32 (gbra)! - it is " << infoHeader.bitsPerPixel << "\n";
		//std::cin.get();
	}


	char compressionMethod[4];
	fin.read(compressionMethod, 4);
	infoHeader.compressionMethod =
		(
			(unsigned char)compressionMethod[0] << 0 |
			(unsigned char)compressionMethod[1] << 8 |
			(unsigned char)compressionMethod[2] << 16 |
			(unsigned char)compressionMethod[3] << 24
			);

	char sizeOfPixelData[4];
	fin.read(sizeOfPixelData, 4);
	infoHeader.sizeOfPixelData =
		(
			(unsigned char)sizeOfPixelData[0] << 0 |
			(unsigned char)sizeOfPixelData[1] << 8 |
			(unsigned char)sizeOfPixelData[2] << 16 |
			(unsigned char)sizeOfPixelData[3] << 24
			);


	assert(infoHeader.remainingHeaderFields.size() == 4); //useless assertion? 

	for (int i = 0; i < infoHeader.remainingHeaderFields.size(); ++i)
	{
		char remainingHeaderFields[4];
		fin.read(remainingHeaderFields, 4);

		infoHeader.remainingHeaderFields.at(i) =
			(
				(unsigned char)remainingHeaderFields[0] << 0 |
				(unsigned char)remainingHeaderFields[1] << 8 |
				(unsigned char)remainingHeaderFields[2] << 16 |
				(unsigned char)remainingHeaderFields[3] << 24
				);

	}
}


/*Note: method is made ghastly long by handling 24 and 32 bit color*/
void ImageBMP::readPixelDataFromFile(ifstream& fin)
{

	if (infoHeader.bitsPerPixel == 32)
	{
		pixelData.pixelMatrix.resize(infoHeader.imageHeight,
			std::vector<Color>(infoHeader.imageWidth));	//CAREFUL to resize as a TWO-d array - NOT 1D!

		for (unsigned int row = 0; row < infoHeader.imageHeight; ++row)
		{
			for (unsigned int col = 0; col < infoHeader.imageWidth; ++col)
			{
				char bgra[4];
				fin.read(bgra, 4);

				if (fin.fail())
					//fin.fail gets set to true if, for example, ... the `row` counter variable goes too far
					//ex: 	for (int row = 0; row < infoHeader.imageHeight + 1; ++row)
				{
					std::cout << "Error: Attempted to read beyond the end of the file at row " << row << ", col " << col << ".\n";
					std::cin.get();
					return;
				}

				Color currentPixelColor
				{ (unsigned int)(unsigned char)bgra[0], (unsigned int)(unsigned char)bgra[1] ,
					(unsigned int)(unsigned char)bgra[2] , (unsigned int)(unsigned char)bgra[3] };
				if (col < infoHeader.imageWidth)
				{
					pixelData.pixelMatrix.at(row).at(col) = currentPixelColor;
				}

				else
				{
					std::cout << "Error: Attempted to access out-of-bounds pixel at row " << row << ", col " << col << ".\n";
					std::cin.get();
					return;
				}

			}
		}

		int lastThingInFile = fin.get(); //should be -1, I think

		//confirm that the end of the file was reached:
		if (!fin.eof())
		{
			std::cout << "Hey!\nListen\n EOF was not reached? Is there more pixel data? \n";
			std::cin.get();
		}
	}

	else
	{
		// Calculate the number of bytes per pixel based on the bit depth
		int bytesPerPixel = infoHeader.bitsPerPixel / 8;

		// Calculate the padding bytes per row (each row is padded to a multiple of 4 bytes)
		int paddingBytes = (4 - (infoHeader.imageWidth * bytesPerPixel) % 4) % 4;

		//cout << "not 32 bits per pixel\n";
		pixelData.pixelMatrix.resize(infoHeader.imageHeight,
			std::vector<Color>(infoHeader.imageWidth));	//CAREFUL to resize as a TWO-d array - NOT 1D!

		for (unsigned int row = 0; row < infoHeader.imageHeight; ++row)
		{
			for (unsigned int col = 0; col < infoHeader.imageWidth; ++col)
			{
				char bgr[3];
				fin.read(bgr, 3);

				if (fin.fail())
					//fin.fail gets set to true if, for example, ... the `row` counter variable goes too far
					//ex: 	for (int row = 0; row < infoHeader.imageHeight + 1; ++row)
				{
					std::cout << "Error: Attempted to read beyond the end of the file at row " << row << ", col " << col << ".\n";
					std::cin.get();
					return;
				}

				Color currentPixelColor
				{ (unsigned int)(unsigned char)bgr[0], (unsigned int)(unsigned char)bgr[1] ,
					(unsigned int)(unsigned char)bgr[2] };
				if (col < infoHeader.imageWidth)
				{
					pixelData.pixelMatrix.at(row).at(col) = currentPixelColor;
				}

				else
				{
					std::cout << "Error: Attempted to access out-of-bounds pixel at row " << row << ", col " << col << ".\n";
					std::cin.get();
					return;
				}

			}
			// Skip the padding bytes at the end of each row
			fin.ignore(paddingBytes);

		}

		//int counter = 0; 
		//while (!fin.eof())
		//{
			//counter++; 
		int lastThingInFile = fin.get(); //should be -1, I think
		//}

		//confirm that the end of the file was reached:
		if (!fin.eof())
		{
			std::cout << "Hey!\nListen\n EOF was not reached? Is there more pixel data? \n";
			std::cin.get();
		}
	}
}

#pragma endregion

#pragma region InfoHeader
unsigned int InfoHeader::getInfoHeaderSize() const
{
	return infoHeaderSize;
}

unsigned int InfoHeader::getSizeOfPixelData() const
{
	//return sizeOfPixelData;
	return imageWidth * imageHeight * (bitsPerPixel / 8);
}
#pragma endregion


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
#pragma endregion 




#pragma region auxillary functions

/*[1, 2, 3] will become
[
1
2
3
]
*/
std::vector<std::vector<char>> rotateMatrixClockwise
	(std::vector<std::vector<char>>& originalMatrix, int originalNumberOfRows, int originalNumberOfCols)
{
	std::vector<std::vector<char>> rotatedMatrix;

	//note the switched order of rows and cols: 
	rotatedMatrix.resize(originalNumberOfCols, vector<char>(originalNumberOfRows));

	for (int row = 0; row < originalNumberOfRows; ++row)
	{
		for (int col = 0; col < originalNumberOfCols; ++col)
		{
			//I didn't work through this equation - only walked through in debug mode...
			rotatedMatrix[col][originalNumberOfRows - 1 - row] = originalMatrix[row][col];
		}
	}


	return rotatedMatrix;
}

/*No great need for this one - just being goofy*/
std::vector<std::vector<int>> rotateIntMatrixClockwise
	(std::vector<std::vector<int>>& originalMatrix, int originalNumberOfRows, int originalNumberOfCols)
{
	std::vector<std::vector<int>> rotatedMatrix;

	//note the switched order of rows and cols: 
	rotatedMatrix.resize(originalNumberOfCols, std::vector<int>(originalNumberOfRows));

	for (int row = 0; row < originalNumberOfRows; ++row)
	{
		for (int col = 0; col < originalNumberOfCols; ++col)
		{
			//I didn't work through this equation - only walked through in debug mode...
			rotatedMatrix[col][originalNumberOfRows - 1 - row] = originalMatrix[row][col];
		}
	}


	return rotatedMatrix;
}



#ifdef __cplusplus
#if __cplusplus >= 201703L
std::vector<ImageBMP> getAllImagesInFolder(std::string folderName)
{
	std::vector<ImageBMP> allImagesInFolder;

	auto startPath = std::filesystem::current_path();
	//set current directory to folder name to get images from: 
	auto newPath = startPath.string() + folderName;
	std::filesystem::current_path(newPath);

	for (auto& entry : std::filesystem::directory_iterator(newPath))
	{
		//cout << entry.path() << "\n";
		//ImageBMP currentPieceImage{}
		ImageBMP currentPieceImage{ entry.path().string() };

		allImagesInFolder.push_back(currentPieceImage);
	}


	//reset path to where main cpp file is
	std::filesystem::current_path(startPath);

	return allImagesInFolder;
}

#endif
#endif

#pragma endregion