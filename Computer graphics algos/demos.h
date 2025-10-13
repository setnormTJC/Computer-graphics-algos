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

void drawMultipleTriangles()
{
	std::array<Vec2, 3> t1Verts = { Vec2{50, 400}, Vec2{150, 400}, Vec2{100, 300} };
	std::array<Vec2, 3> t2Verts = { Vec2{200, 450}, Vec2{300, 450}, Vec2{250, 350} };
	std::array<Vec2, 3> t3Verts = { Vec2{350, 400}, Vec2{450, 400}, Vec2{400, 300} };
	std::array<Vec2, 3> t4Verts = { Vec2{150, 200}, Vec2{250, 200}, Vec2{200, 100} };

	Color c1 = { 255, 0, 0 };     // red
	Color c2 = { 0, 255, 0 };     // green
	Color c3 = { 0, 0, 255 };     // blue
	Color c4 = { 255, 255, 0 };   // yellow

	Triangle t1(t1Verts);
	Triangle t2(t2Verts);
	Triangle t3(t3Verts);
	Triangle t4(t4Verts);

	ImageBMP image(500, 500, Color(0, 0, 0));

	image.drawFilledTriangle(t1.getPointsThatFillTriangle(), c1);
	image.drawFilledTriangle(t2.getPointsThatFillTriangle(), c2);
	image.drawFilledTriangle(t3.getPointsThatFillTriangle(), c3);
	image.drawFilledTriangle(t4.getPointsThatFillTriangle(), c4);

	std::string filename = "maybePrettyImage.bmp";

	image.writeImageFile(filename);

	std::system(filename.c_str());

}