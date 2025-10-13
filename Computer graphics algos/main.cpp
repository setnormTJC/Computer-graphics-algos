// Computer graphics algos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<array> 
#include <vector>

#include"ImageBMP.h"

#include"Triangle.h"

#include"Demos.h"

int main()
{
	try
	{
		//general example (gets split into two triangles - a flat top and a flat bottom): 
		Vec2 v0 = { 0, 0 };
		Vec2 v1 = { 7, 0 };
		Vec2 v2 = { 0, 8 }; 

		Triangle triangle({v0, v1, v2});

		//std::cout << "Edge length: " << triangle.getEdgeLength(Edge(v0, v1));
		
		auto edges = triangle.getEdges(); 

		int indexOfFirstEdge = 0; 
		int indexOfSecondEdge = 1; 

		std::cout << "Angle between edges " << edges[indexOfFirstEdge] << " and " 
			<< edges[indexOfSecondEdge]
			<< ": " << triangle.getAngleOfAdjacentEdges(indexOfFirstEdge, indexOfSecondEdge)
			<< " degrees\n";


		//__debugbreak(); 


		//auto dims = triangle.getBoundingBoxDimensions(); 
		//ImageBMP image(dims.width, dims.height, Color(0, 0, 0));

		//auto points = triangle.getPointsThatFillTriangle(); 
		//image.drawFilledTriangle(points, Color(255, 255, 255));

		//std::string filename = "image.bmp"; 
		//image.writeImageFile(filename);

		//std::system(filename.c_str());

		//demoFlatBottomAndFlatTopTriangles(); 

		
	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

