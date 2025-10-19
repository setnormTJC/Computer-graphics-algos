// Computer graphics algos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _USE_MATH_DEFINES

#include<array> 
#include<chrono>
#include<fstream> 
#include<iostream>
#include<thread> 
#include<vector>

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
//#include "CoordinateTransformer.h"
#include "Cube.h"
#include "Camera.h"



int main()
{
	try
	{
		constexpr int screenWidth = 500;
		constexpr int screenHeight = 500;
		
		Cube cube{}; 

		auto cubeVerts = cube.getCubeVerts();

		Camera camera;

		auto screenSpaceCubeVerts = camera.projectToScreen(cubeVerts, screenWidth, screenHeight);

		auto rasterPoints = cube.rasterize(screenSpaceCubeVerts);
			
		ImageBMP image(screenWidth, screenHeight, ColorEnum::Black);

		image.fillPixelMatrix(rasterPoints, ColorEnum::Purple);

		image.saveAsPNG("Camera_based_cube" + Utils::getTimestampForFilename() + ".png");

	}

	catch (const MyException& e)
	{
		std::cout << e.whatWentWrong() << "\n";
	}
}

