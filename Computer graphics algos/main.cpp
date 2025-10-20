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
		Vec4 rot(0.0f, M_PI/4, 0.0f, 0.0f); //units: radians, rotate about y-axis 45 degrees (w = 0.0f here?) 
		Vec4 scale(1.0f, 1.0f, 1.0f, 0.0f); //again, not sure if w = 0.0f or 1.0f here 
		Vec4 trans(1.0f, 0.0f, -2.0f, 1.0f); //ensure that z < 0.0f  (default camera pos) for zInit + zTrans

		Mesh mesh(localCubeVerts, trans, rot, scale);

		std::vector<Vec4> worldCubeVerts = mesh.applyModelMatrix(); 

		float fovY = (M_PI / 2);
		Camera camera(fovY);

		auto screenSpaceCubeVerts = camera.projectToScreen(worldCubeVerts, screenWidth, screenHeight);

		auto rasterPoints = cube.rasterize(screenSpaceCubeVerts);


		ImageBMP image(screenWidth, screenHeight, ColorEnum::Black);

		image.fillPixelMatrix(rasterPoints, ColorEnum::Purple);

		image.saveAsPNG("Cube_with_transformations_" + Utils::getTimestampForFilename() + ".png");
		//camera.logCameraInfo("CameraLog" + Utils::getTimestampForFilename() + ".log");			
	}

	catch (const MyException& e)
	{
		std::cout << e.whatWentWrong() << "\n";
	}
}

