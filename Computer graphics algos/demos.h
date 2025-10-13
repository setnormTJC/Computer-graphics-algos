#pragma once

#include <iostream>
#include<array> 
#include <vector>

#include"ImageBMP.h"

#include"Triangle.h"

#include"Demos.h"

void demoFlatBottomAndFlatTopTriangles()
{
	//flat bottom example:
	//Vec2 v0 = { 0, 8 };
	//Vec2 v1 = { 9, 0 };
	//Vec2 v2 = { 3, 0 }; //deliberately "disordered" to test the lexicographic sort in Triangle's parameterized constructor def.

	//flat top example:
	Vec2 v0 = {200, 0};
	Vec2 v1 = {0, 500}; 
	Vec2 v2 = {200, 500}; //ordered because I am more or less sure that lexico sort is working


	Triangle triangle({ v0, v1, v2 });

	std::vector<Vec2> filledPoints = triangle.getPointsThatFillTriangle();

	/*Makes a border 10 pixels above, below, left, and right of the triangle*/
	constexpr int PADDING = 10;
	int width = triangle.getBoundingBoxDimensions().width + PADDING;
	int height = triangle.getBoundingBoxDimensions().height + PADDING;
	Color bgrdColor = Color(0, 0, 0);

	ImageBMP image(width, height, bgrdColor);

	Color triangleColor = { 255, 255, 255 };
	image.drawFilledTriangle(filledPoints, triangleColor);

	std::string filename = "image.bmp";

	image.writeImageFile(filename);

	std::system(filename.c_str());
}