// Computer graphics algos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<array> 
#include <vector>

#include"ImageBMP.h"

#include"Triangle.h"

#include"Demos.h"


std::string getFileTimestamp()
{
	std::string time = __TIME__; 

	std::replace(time.begin(), time.end(), ':', '_');

	return time; 
}

int main()
{
	try
	{
		//general example (gets split into two triangles - a flat top and a flat bottom): 
		Vec2 v0 = { 000, 000 };
		Vec2 v1 = { 700, 300 };
		Vec2 v2 = { 200, 800 }; 

		Triangle triangle({v0, v1, v2});
		


		auto dims = triangle.getBoundingBoxDimensions(); 
		const int PADDING = 10; 
		ImageBMP image(dims.width + PADDING, dims.height + PADDING, Color(0, 0, 0));

		auto points = triangle.getPointsThatFillTriangle(); 
		image.drawFilledTriangle(points, Color(255, 255, 255));


		std::string filename = getFileTimestamp() + ".bmp";
		image.writeImageFile(filename);

		std::system(filename.c_str());

		
	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

