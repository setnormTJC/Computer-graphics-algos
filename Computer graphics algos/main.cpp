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
#include "MyException.h"
#include"Polygon.h"
#include"Triangle.h"
#include "Vec4.h"
#include "Mat4.h"
#include "Mesh.h"
#include "Camera.h"
#include "SDLWrapper.h"


int main()
{
	try
	{
		/*Vertices of object to draw*/
		//Mesh mesh(CommonPolyhedronType::tetrahedron);
		Mesh mesh(CommonPolyhedronType::octahedron);

		std::vector<Vec4> localVerts = mesh.getLocalVertices(); 

		/*Set the initial transformations to be applied to object*/
		Vec4 rot(0.0f,
			0.0f,
			0.0f,
			0.0f);

		Vec4 scale(1.0f, 1.0f, 1.0f, 0.0f);
		Vec4 trans(0.0f, 0.0f, -5.0f, 1.0f);

		/*Apply the transformations and get screen-equivalent coordinates*/
		MeshInstance meshInstance(trans, rot, scale);

		/*Colors to draw object with*/
		std::vector<Color> colors;
		auto colorEnums = Color::getBroadColorPalette();
		for (const auto& current : colorEnums)
		{
			colors.push_back(Color(current));
		}

		int screenWidth = 1'000; 
		int screenHeight = 1'000; 
		
		Camera camera(screenWidth, screenHeight);
		
		SDLWrapper sdlWrapper(screenWidth, screenHeight); 

		/*Main loop*/
		return sdlWrapper.run(mesh, meshInstance, camera, localVerts, colors);
	}

	catch (const MyException& e)
	{
		std::cout << e.whatWentWrong() << "\n";
	}
}


