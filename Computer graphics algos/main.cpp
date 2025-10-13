// Computer graphics algos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<array> 
#include <vector>

#include"ImageBMP.h"

#include"Triangle.h"

#include"Demos.h"


std::string getFileTimestamp()
{
	std::string time = __TIME__; 

	std::replace(time.begin(), time.end(), ':', '_');

	return time; 
}

int main()
{
	try
	{
        ImageBMP image(500, 500, Color(0, 0, 0));

        std::vector<std::pair<Triangle, Color>> triangles = {
            { Triangle({ Vec2{50, 400}, Vec2{150, 400}, Vec2{100, 300} }), Color(255, 0, 0) },
            { Triangle({ Vec2{200, 100}, Vec2{250, 200}, Vec2{300, 100} }), Color(0, 255, 0) },
            { Triangle({ Vec2{350, 400}, Vec2{450, 300}, Vec2{400, 200} }), Color(0, 0, 255) },
            { Triangle({ Vec2{50, 250}, Vec2{100, 150}, Vec2{150, 250} }), Color(255, 255, 0) },
            { Triangle({ Vec2{200, 350}, Vec2{300, 350}, Vec2{250, 450} }), Color(255, 0, 255) },
            { Triangle({ Vec2{350, 100}, Vec2{400, 200}, Vec2{450, 100} }), Color(0, 255, 255) },
            { Triangle({ Vec2{100, 50}, Vec2{50, 150}, Vec2{150, 200} }), Color(255, 128, 0) },
            { Triangle({ Vec2{250, 250}, Vec2{350, 250}, Vec2{300, 150} }), Color(128, 0, 255) },
            { Triangle({ Vec2{400, 450}, Vec2{450, 350}, Vec2{350, 350} }), Color(0, 128, 255) },
            { Triangle({ Vec2{200, 250}, Vec2{150, 200}, Vec2{250, 150} }), Color(128, 128, 128) }
        };

        for (auto& [triangle, color] : triangles)
        {
            std::vector<Vec2> points = triangle.getPointsThatFillTriangle();
            for (const auto& p : points)
            {
                image.drawFilledTriangle(points, color);
            }
        }

        std::string filename = "trianglesGalore.bmp";
        image.writeImageFile(filename);

        std::system(filename.c_str());

	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

