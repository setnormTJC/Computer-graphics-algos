#pragma once

#include"ImageBMP.h"
#include"Triangle.h"

class GraphicsDemo
{
private: 
	std::vector<Vec2> pixels;
	int imageWidth{}, imageHeight{};

public: 
	/*
	* @param numberOfRows -> I SUPPOSE a "checkerboard" usually has the same number of rows as columns
	*/
	void fillPointsOfCheckerboard(const int numberOfRows, const int numberOfCols, const int sideLength);

	/*A FILLED-IN square, that is @returns nothing - it modifies the member variable `pixels`
	* @param xPos and yPos -> the coordinates of the lower left vertex of the square 
	*/
	void fillPointsOfSquare(const int xPos, const int yPos, const int sideLength);

	void fillPointsOfEquilateralTriangle(const Edge& edgeOfEquilateralTriangle);

	void fillPointsOfSierpinski(const Edge& baseEdge, int depth, const Color& color);


	void draw(const std::string& filename);
};

