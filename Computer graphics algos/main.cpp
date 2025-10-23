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
#include "Cube.h"
#include "Camera.h"
#include "Mesh.h"
#include "SDLWrapper.h"


int main()
{
	try
	{
		/*Vertices of object to draw*/
		Cube cube{};
		std::vector<Vec4> localCubeVerts = cube.getLocalCubeVerts();

		/*Colors to draw object with*/
		std::vector<Color> colors;
		auto colorEnums = Color::getBroadColorPalette();
		for (const auto& current : colorEnums)
		{
			colors.push_back(Color(current));
		}

		/*Camera setup*/
		float fovY = (M_PI / 2);
		Camera camera(fovY);

		/*Screen dims and SDL wrapper*/
		int width = 1000;
		int height = 1000;
		SDLWrapper sdlWrapper(width, height); 

		/*Main loop*/
		return (sdlWrapper.run(cube, camera, localCubeVerts, colors)); 

	}

	catch (const MyException& e)
	{
		std::cout << e.whatWentWrong() << "\n";
	}
}


