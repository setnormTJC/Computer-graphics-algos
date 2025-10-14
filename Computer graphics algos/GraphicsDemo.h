#pragma once

#include"ImageBMP.h"
#include"Triangle.h"

class GraphicsDemo
{
private: 
	std::vector<Vec2> pixels;
	int imageWidth, imageHeight;

public: 
	void getPointsOfCheckerboard(); 
	/*A FILLED-IN square, that is @returns nothing - it modifies the member variable `pixels`*/
	void getPointsOfSquare(const int xPos, const int yPos, const int sideLength);

	void draw(const std::string& filename);
};

