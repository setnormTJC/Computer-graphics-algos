#pragma once

#include"ImageBMP.h"
#include"Triangle.h"

class GraphicsDemo
{
private:
	//std::vector<Vec2> points;

	std::unordered_map<Vec2, Color> pointsToColors;

	int imageWidth{}, imageHeight{};

public:
	GraphicsDemo();
	GraphicsDemo(const std::vector<Vec2>& points, const Color& colorOfAllPixels);
	GraphicsDemo(const std::vector<Triangle>& triangles, const std::vector<ColorEnum>& colors);
	GraphicsDemo(const std::unordered_map<Vec2, Color>& pointsToColors);


	void draw(const std::string& filename);


private:
	/*Called if `points` arg constructor is called*/
	void fillPointsToColorsMap(const std::vector<Vec2>& points, const Color& color);
};

