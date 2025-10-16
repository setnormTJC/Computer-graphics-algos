// Computer graphics algos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<array> 
#include <vector>

#include"ImageBMP.h"

#include"Triangle.h"

#include"Demos.h"
#include "GraphicsDemo.h"


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
		GraphicsDemo graphicsDemo;

		//Edge AB = { Vec2(0, 0), Vec2(0, 50) }; //test case for +60 degree rotation into negative x axis for third vertex
		Edge AB = { Vec2(0, 0), Vec2(50, 0) };

		Triangle rootTriangle(AB);
		
		graphicsDemo.fillPointsOfSierpinski(rootTriangle, 1, Color(255, 223, 0));
		graphicsDemo.draw("Triforce-of-course.png");

		//Triangle t(AB);

	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

