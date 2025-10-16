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

		////graphicsDemo.getPointsOfSquare(100, 200, 50); 
		////graphicsDemo.fillPointsOfCheckerboard(8, 8, 50); 

		//graphicsDemo.draw(getTimestampForFilename() + ".png"); //PNG now!
		Edge AB = { Vec2(0, 0), Vec2(0, 50) };

		//graphicsDemo.fillPointsOfEquilateralTriangle(AB); 

		//graphicsDemo.draw("equilateralTriangle.png");
		graphicsDemo.fillPointsOfSierpinski(AB, 3, Color(255, 255, 255));


		graphicsDemo.draw("sierpinski.png");
	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

