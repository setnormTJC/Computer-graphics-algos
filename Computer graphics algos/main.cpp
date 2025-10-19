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

		Cube c;
		auto normalizedCubeVerts = c.getCubeVerts(); 

		constexpr int screenWidth = 500;
		constexpr int screenHeight = 500;

		CoordinateTransformer ct(normalizedCubeVerts);
		
		float zFar = 2.0f; 
		float zNear = 1.0f; 
		float fovY = M_PI / 2; 

		auto screenSpaceCubeVerts = ct.getScreenSpaceVerts(zFar, zNear, screenWidth, screenHeight, fovY);

		auto rasterPoints = c.rasterize(screenSpaceCubeVerts);
		ImageBMP image(screenSpaceCubeVerts, ColorEnum::DarkerGreen);
		image.fillPixelMatrix(rasterPoints);
		image.saveAsPNG("cube" + Utils::getTimestampForFilename() + ".png");
	}

	catch (const MyException& e)
	{
		std::cout << e.whatWentWrong() << "\n";
	}
}

