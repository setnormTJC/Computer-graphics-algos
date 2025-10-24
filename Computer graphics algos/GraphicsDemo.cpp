//#include "GraphicsDemo.h"
//
//#include<tuple>
//#include "MyException.h"
//
//GraphicsDemo::GraphicsDemo() = default;
//
//GraphicsDemo::GraphicsDemo(const std::vector<Vec2>& points, const Color& colorOfAllPixels)
//{
//	fillPointsToColorsMap(points, colorOfAllPixels); 
//}
//
//GraphicsDemo::GraphicsDemo(const std::vector<Triangle>& triangles, const std::vector<ColorEnum>& colors)
//{
//	if (colors.size() == 0)
//	{
//		throw MyException("size of colors array cannot be 0", __LINE__, __FILE__);
//	}
//	//"unpack" the triangles
//	for (int i = 0; i < triangles.size(); ++i)
//	{
//		auto pts = triangles.at(i).getPointsThatFillTriangle();
//		fillPointsToColorsMap(pts, colors.at(i % colors.size())); //wrap around to beginning of colors if more triangles than colors 
//	}
//}
//
//GraphicsDemo::GraphicsDemo(const std::unordered_map<Vec2, Color>& pointsToColors)
//	:pointsToColors(pointsToColors)
//{
//
//}
//
//void GraphicsDemo::draw(const std::string& filename)
//{
//	//get dimensions of bounding box
//	//points to colors -> a std::unordered_map<Vec2, Color>
//	auto xMax = (*pointsToColors.begin()).first.x; //wonderfully horrific - use C++20's ranges, perhaps?
//	auto yMax = (*pointsToColors.begin()).first.y;
//
//	for (const auto& [point, color] : pointsToColors)
//	{
//		if (point.x > xMax) xMax = point.x;
//		if (point.y > yMax) yMax = point.y; 
//	}
//
//	Box2D dims(xMax, yMax);
//
//
//	constexpr int PADDING = 1; //PADDING OF 1 to handle floating point error and off by one errors ... hopefully 
//	imageWidth += dims.width + PADDING;
//	imageHeight += dims.height + PADDING;
//
//	Color bgrdColor(0, 0, 0);
//
//	ImageBMP image(imageWidth, imageHeight, bgrdColor);
//
//	image.fillPixelMatrix(pointsToColors);
//
//	image.saveAsPNG(filename);
//
//	std::system(filename.c_str());
//}
//
//
//
//void GraphicsDemo::fillPointsToColorsMap(const std::vector<Vec2>& points, const Color& color)
//{
//	for (const auto& point : points)
//	{
//		pointsToColors.insert({ point, color });
//	}
//}
//
//
//
//
//
