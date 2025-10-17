// Computer graphics algos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include<array> 
#include <iostream>
#include <vector>

#include"Demos.h"
#include "GraphicsDemo.h"
#include"ImageBMP.h"
#include"Polygon.h"
#include"Triangle.h"


std::string getTimestampForFilename()
{
	std::string time = __TIME__; 

	std::replace(time.begin(), time.end(), ':', '_');

	return time; 
}



int main()
{
	try
	{
		Edge AB = { Vec2(0, 0), Vec2(1000, 0) };

		Triangle rootTriangle(AB);

		int triangleCount = 0;

		GraphicsDemo graphicsDemo;
		graphicsDemo.fillPointsOfSierpinski(rootTriangle, 2, ColorEnum::Red, triangleCount);
		graphicsDemo.draw("variousColorSierpinski.png");
	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

