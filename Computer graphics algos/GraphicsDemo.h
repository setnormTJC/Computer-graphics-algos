#pragma once

#include"ImageBMP.h"
#include"Triangle.h"

class GraphicsDemo
{
private: 
	std::vector<Vec2> points;
	
	std::unordered_map<Vec2, Color> pointsToColors; 

	int imageWidth{}, imageHeight{};

public: 
	GraphicsDemo(); 
	GraphicsDemo(const std::vector<Vec2>& points, const Color& colorOfAllPixels);

	GraphicsDemo(const std::unordered_map<Vec2, Color>& pointsToColors);

	/*
	* @param numberOfRows -> I SUPPOSE a "checkerboard" usually has the same number of rows as columns
	*/
	void fillPointsOfCheckerboard(const int numberOfRows, const int numberOfCols, const int sideLength);

	/*A FILLED-IN square, that is @returns nothing - it modifies the member variable `pixels`
	* @param xPos and yPos -> the coordinates of the lower left vertex of the square 
	*/
	void fillPointsOfSquare(const int xPos, const int yPos, const int sideLength);

	void fillPointsOfEquilateralTriangle(const Edge& edgeOfEquilateralTriangle);

	void fillPointsOfSierpinski(const Triangle& tri, int depth, const Color& color/*, int& triangleCount*/);

	void draw(const std::string& filename);


private: 
	void fillPointsToColorsMap(const Color& colorOfAllPixels); 
	
	void fillPoints(); 

};

