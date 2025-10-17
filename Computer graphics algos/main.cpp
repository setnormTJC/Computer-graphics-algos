// Computer graphics algos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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



int main()
{
	try
	{
		Vec2 A(20, 20);
		Vec2 B(80, 20);
		Vec2 C(20, 80);
		Vec2 D(80, 80);
		Vec2 E(100, 50); 
		//Vec2 E(50, 50); //results in a non-simple polygon 

		std::vector<Vec2> verts =
		{
			A,
			B,
			C,
			D,
			E  //HERE!
		};

		Polygon bowtie(verts);

		auto triangles = bowtie.triangulate();
		auto colors = Color::getGrayColorPalette(); 

		GraphicsDemo gd(triangles, colors); 

		std::string filename = getTimestampForFilename() + ".png";
		gd.draw(filename);

	}

	catch (const MyException& e)
	{
		std::cout << e.whatWentWrong() << "\n";
	}
}

