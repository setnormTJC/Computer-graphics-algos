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
#include "Texture.h"


int main()
{
	try
	{
		//Texture texture("bmpOutputs/checkerboard.bmp");//this is POSSIBLY just a temporary (expensive) approach

		/*Vertices of object to draw*/
		Mesh mesh(CommonPolyhedronType::cube);
		//Mesh mesh(CommonPolygonType::isocelesTriangle);
		//Mesh mesh(CommonPolygonType::rectangle);

		std::vector<Vec4> localVerts = mesh.getLocalVertices(); 


		/*Set the initial transformations to be applied to object*/
		Vec4 rot(0.0f,
			0.0f,
			0.0f,
			0.0f);

		Vec4 scale(1.0f, 1.0f, 1.0f, 0.0f);
		Vec4 trans(0.0f, 0.0f, -3.0f, 1.0f);  //set z = -3.0 or so here for polyhedrons with their current coords

		/*Apply the transformations and get screen-equivalent coordinates*/
		MeshInstance meshInstance(trans, rot, scale);

		/*Colors to draw object with*/
		std::vector<Color> colors;
		auto colorEnums = Color::getBroadColorPalette();
		for (size_t i = 0; i < mesh.getLocalVertices().size(); ++i) //make the number of colors = the number of verts (in the Mesh object) 
		{
			Color currentColor = colorEnums[i % colorEnums.size()];
			colors.push_back(currentColor);
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


