// Computer graphics algos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include<array> 
#include <iostream>
#include <vector>

#include"Edge.h"
#include"GraphicsDemo.h"
#include"ImageBMP.h"
#include"Polygon.h"
#include"Triangle.h"
#include"UnorganizedDemos.h"



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

		auto trianglesMakingUpPentagon = polygon.triangulate(); 

		int triangleCount = 0;
		auto palette = Color::getGrayColorPalette(); 

		std::vector<Vec2> points;
		std::unordered_map<Vec2, Color> pointsToColors; 

		for (const auto& triangle : trianglesMakingUpPentagon)
		{
			//auto currentTrianglePoints = triangle.getPointsThatOutlineTriangle(); 
			auto currentTrianglePoints = triangle.getPointsThatFillTriangle(); 

			points.insert(points.begin(), currentTrianglePoints.begin(), currentTrianglePoints.end()); 

			for (const auto& point : currentTrianglePoints)
			{
				pointsToColors.insert({ point, Color(palette[triangleCount])});
			}
			triangleCount++; 
		}

		GraphicsDemo gd(pointsToColors); 
		gd.draw("filledInTrianglesOfPentagram-grayPalette.png");
	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

