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
#include "Mesh.h"



int main()
{
	try
	{
		constexpr int screenWidth = 500;
		constexpr int screenHeight = 500;


		Cube cube{};

		std::vector<Vec4> localCubeVerts = cube.getLocalCubeVerts();

		/*Construct a Mesh that (possibly) applies transformations (rot, scale, trans) on the cube
		* - this converts from "local" to "world" coordinates
		*/
		Vec4 rot(M_PI / 4, M_PI/6, M_PI/5, 0.0f); //units: radians, rotate about y-axis 45 degrees (w = 0.0f here?) 
		Vec4 scale(1.0f, 1.0f, 1.0f, 0.0f); //again, not sure if w = 0.0f or 1.0f here 
		Vec4 trans(0.0f, 0.0f, -3.0f, 1.0f); //ensure that z < 0.0f  (default camera pos) for zInit + zTrans

		Mesh mesh(localCubeVerts, trans, rot, scale);

		std::vector<Vec4> worldCubeVerts = mesh.applyModelMatrix(); 

		float fovY = (M_PI / 2);
		Camera camera(fovY);

		auto screenSpaceCubeVerts = camera.projectToScreen(worldCubeVerts, screenWidth, screenHeight);

		std::vector<Color> colors; 
		auto colorEnums = Color::getGrayColorPalette(); 
		for (const auto& current : colorEnums)
		{
			colors.push_back(Color(current));
		}
		//auto rasterPoints = cube.rasterize(screenSpaceCubeVerts);
		auto rasteredPixels = cube.rasterize(screenSpaceCubeVerts, colors);

		ImageBMP image(screenWidth, screenHeight, ColorEnum::Pink);

		//image.fillPixelMatrix(rasteredPoints, ColorEnum::Purple);

		image.fillPixelMatrix(rasteredPixels);

		image.saveAsPNG("Cube_with_transformations_" + Utils::getTimestampForFilename() + ".png");
		//camera.logCameraInfo("CameraLog" + Utils::getTimestampForFilename() + ".log");			
	}

	catch (const MyException& e)
	{
		std::cout << e.whatWentWrong() << "\n";
	}
}

