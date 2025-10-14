#include "GraphicsDemo.h"

void GraphicsDemo::getPointsOfCheckerboard()
{

}

void GraphicsDemo::getPointsOfSquare(const int xPos, const int yPos, const int sideLength)
{
	std::array<Vec2, 3> leftTriangleVertices =
	{
		Vec2(xPos, yPos),
		Vec2(xPos + sideLength, yPos),
		Vec2(xPos + sideLength, yPos + sideLength)
	};

	Triangle leftTriangle(leftTriangleVertices);
	auto dims = leftTriangle.getBoundingBoxDimensions(); 
	auto fillpoints = leftTriangle.getPointsThatFillTriangle(); 


	/*Update member vars*/
	constexpr int PADDING = 10;
	imageWidth = dims.width + xPos + PADDING; 
	imageHeight = dims.height + yPos + PADDING; 


	pixels.insert(pixels.end(), fillpoints.begin(), fillpoints.end());

}

void GraphicsDemo::draw(const std::string& filename)
{

	ImageBMP image(imageWidth, imageHeight, Color(0, 0, 0));

	image.drawFilledTriangle(pixels, Color(255, 0, 255)); 

	image.writeImageFile(filename); 

	std::system(filename.c_str());
}
