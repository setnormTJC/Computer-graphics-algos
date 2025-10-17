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

void giveMeAPentagon()
{
	std::vector<Vec2> regularPentagonVerts =
	{
		Vec2(90, 30), //deliberately NOT in CW or CCW order
		Vec2(60, 50),
		Vec2(80, 00),
		Vec2(50, 00),
		Vec2(40, 30)
	};

	Polygon polygon(regularPentagonVerts);

	//auto approxCentroid = polygon.getApproximateCentroid(); 
	auto triangles = polygon.triangulate();

	std::vector<Vec2> fillpoints;

	for (const auto& triangle : triangles)
	{
		auto currentFillpoints = triangle.getPointsThatFillTriangle();

		fillpoints.insert(
			fillpoints.end(), currentFillpoints.begin(), currentFillpoints.end());


	}

	GraphicsDemo graphicsDemo(fillpoints);
	graphicsDemo.draw("possiblyAPentagon.png");
}

int main()
{
	try
	{
		std::vector<Vec2> pentagramVerts =
		{
			Vec2(50, 100),   // top outer
			Vec2(65, 65),    // inner right
			Vec2(100, 65),   // outer right
			Vec2(70, 45),    // inner bottom-right
			Vec2(80, 0),     // outer bottom-right
			Vec2(50, 25),    // inner bottom
			Vec2(20, 0),     // outer bottom-left
			Vec2(30, 45),    // inner bottom-left
			Vec2(0, 65),     // outer left
			Vec2(35, 65)     // inner left
		};
	
		Polygon polygon(pentagramVerts);

		//auto approxCentroid = polygon.getApproximateCentroid(); 
		auto triangles = polygon.triangulate();

		std::vector<Vec2> fillpoints;

		for (const auto& triangle : triangles)
		{
			auto currentFillpoints = triangle.getPointsThatFillTriangle();

			fillpoints.insert(
				fillpoints.end(), currentFillpoints.begin(), currentFillpoints.end());


		}

		GraphicsDemo graphicsDemo(fillpoints);
		graphicsDemo.draw("possiblyAPentagram.png");
	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

