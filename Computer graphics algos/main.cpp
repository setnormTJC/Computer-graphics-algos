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

		graphicsDemo.getPointsOfSquare(100, 200, 50); 

		graphicsDemo.draw(getTimestampForFilename() + ".bmp");

	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

