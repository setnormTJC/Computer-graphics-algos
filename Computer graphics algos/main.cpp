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
		std::vector<Vec2> verts =
		{
			Vec2(1, 2),
			Vec2(3, 3),
			Vec2(5, 1),

		};

		Polygon polygon(verts); 

	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

