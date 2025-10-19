// Computer graphics algos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _USE_MATH_DEFINES

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
#include "Cube.h"



int main()
{
	try
	{
		constexpr int screenWidth = 500;
		constexpr int screenHeight = 500;
		ImageBMP image(screenWidth, screenHeight, ColorEnum::Black);

		float zFar = 3.0f; 
		float zNear = 1.0f; 
		float fovY = M_PI / 2; 

		auto colors = Color::getBroadColorPalette(); 
		/*Loopy*/
		for (int i = 0; i < 5; ++i)
		{
			float xPos = -1.2f + 0.9f * i;  // spread along x-axis
			float yPos = 0.0f;              // all centered vertically
			float zOffset = -1.5f - i * 1.0f; // step farther back each time
			float scale = 0.3f;             // uniform scale for clarity

			Cube c(xPos, yPos, scale, zOffset); // (we'll tweak constructor below)

			auto normalizedCubeVerts = c.getCubeVerts();

			CoordinateTransformer ct(normalizedCubeVerts);

			auto screenSpaceCubeVerts = ct.getScreenSpaceVerts(zFar, zNear, screenWidth, screenHeight, fovY);

			auto rasterPoints = c.rasterize(screenSpaceCubeVerts);

			image.fillPixelMatrix(rasterPoints, colors[i]);
		}
		/*end loopy*/

		image.saveAsPNG("ROW_of_cubes_" + Utils::getTimestampForFilename() + ".png");
	}

	catch (const MyException& e)
	{
		std::cout << e.whatWentWrong() << "\n";
	}
}

