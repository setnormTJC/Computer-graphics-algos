// Computer graphics algos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include<array> 
#include <iostream>
#include <vector>

#include"Edge.h"
#include"GraphicsDemo.h"
#include"ImageBMP.h"
#include "MyException.h"
#include"Polygon.h"
#include"Triangle.h"
#include"UnorganizedDemos.h"
#include"Utils.h"
#include "Vec4.h"
#include "Mat4.h"
#include "CoordinateTransformer.h"



int main()
{
	try
	{
		float xMin = -1.0f; 
		float yMin = -1.0f; 
		float zNear = 1.0f; 

		float xMax = 1.0f; 
		float yMax = 1.0f; 
		float zFar = 2.0f;  


		float wAlways = 1.0f;

		//NORMALIZED
		std::vector<Vec4> normalizedCubeVerts =
		{
			{xMin, yMin, zNear, wAlways}, //back bottom left
			{xMin, yMax, zNear, wAlways}, //back top left
			{xMax, yMin, zNear, wAlways}, //back bottom right
			{xMax, yMax, zNear, wAlways}, //back top right
			
			{xMin, yMin, zFar, wAlways}, //front bottom left
			{xMin, yMax, zFar, wAlways}, //front top left
			{xMax, yMin, zFar, wAlways}, //front bottom right
			{xMax, yMax, zFar, wAlways}, //front top right
		};

		const std::vector<std::pair<int, int>> cubeEdges = {
			{0,1}, {0,2}, {1,3}, {2,3}, // back face
			{4,5}, {4,6}, {5,7}, {6,7}, // front face
			{0,4}, {1,5}, {2,6}, {3,7}  // connecting edges
		};


		//float zCamera = 200.0f; //come back to it
		//also fov

		constexpr int screenWidth = 200;
		constexpr int screenHeight = 200;

		CoordinateTransformer ct(normalizedCubeVerts);
		auto screenSpaceCubeVerts = ct.getScreenSpaceVerts(zFar, zNear, screenWidth, screenHeight); 

		for (const auto& vert : screenSpaceCubeVerts)
		{
			std::cout << vert << "\n";
		}


		auto boundingBox = Utils::getExtrema(screenSpaceCubeVerts);
		const int PADDING = 2; 

		int width = boundingBox[1].x - boundingBox[0].x + PADDING;
		int height = boundingBox[1].y - boundingBox[0].y + PADDING; 


		Color bgrdColor = ColorEnum::Black;
		ImageBMP image(width, height, bgrdColor);

		/*get edges (12 of em)*/
		
		std::vector<Edge> edges;
		for (auto& [i, j] : cubeEdges)
		{
			auto a = screenSpaceCubeVerts.at(i); 
			auto b = screenSpaceCubeVerts.at(j);
			edges.push_back(Edge(a, b));
		}

		for (int i = 0; i < edges.size(); ++i)
		{
			auto line = edges[i].getPointsOfLineSegment(); 
			screenSpaceCubeVerts.insert(screenSpaceCubeVerts.end(), line.begin(), line.end());
		}

		std::unordered_map<Vec2, Color> pointsToColors; 
	
		/*Then the thing of interest:*/
		auto colors = Color::getGrayColorPalette(); 
		int i = 0; 
		for (const auto& vert : screenSpaceCubeVerts)
		{
			//pointsToColors[{vert.x, vert.y}] = colors[i % colors.size()];
			pointsToColors[{vert.x, vert.y}] = ColorEnum::Green;
			i++; 
		}

		image.fillPixelMatrix(pointsToColors);

		image.saveAsPNG("cube_maybe" + Utils::getTimestampForFilename() + ".png");
	}

	catch (const MyException& e)
	{
		std::cout << e.whatWentWrong() << "\n";
	}
}

