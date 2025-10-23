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
	int width = 500; 
	int height = 500; 

	Cube cube{};
	std::vector<Vec4> localCubeVerts = cube.getLocalCubeVerts();

	float fovY = (M_PI / 2);

	Camera camera(fovY);


	SDLWrapper sdlWrapper(width, height); 

	return (sdlWrapper.run(cube, camera, localCubeVerts)); 
}


