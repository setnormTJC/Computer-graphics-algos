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
#include "Mesh.h"
#include "Camera.h"
#include "Mesh.h"
#include "SDLWrapper.h"


int main()
{
	try
	{
		/*Vertices of object to draw*/
		Mesh mesh(CommonPolyhedronType::tetrahedron);

		std::vector<Vec4> localVerts = mesh.getLocalVertices(); 

		/*Colors to draw object with*/
		std::vector<Color> colors;
		auto colorEnums = Color::getGreenColorPalette(); 
		for (const auto& current : colorEnums)
		{
			colors.push_back(Color(current));
		}

		int screenWidth = 1'000; 
		int screenHeight = 1'000; 
		
		Camera camera(screenWidth, screenHeight);
		
		SDLWrapper sdlWrapper(screenWidth, screenHeight); 

		/*Main loop*/
		return sdlWrapper.run(mesh, camera, localVerts, colors);
	}

	catch (const MyException& e)
	{
		std::cout << e.whatWentWrong() << "\n";
	}
}


