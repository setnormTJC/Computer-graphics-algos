//#pragma once
//
//#include<array> 
//#include <iostream>
//#include<string> 
//#include <vector>
//
//#include"GraphicsDemo.h"
//#include"ImageBMP.h"
//#include"Polygon.h"
//#include"Triangle.h"
//
//
///*Component triangles are rendered in shades of gray*/
//void demoPentagramTriangulation()
//{
//	std::vector<Vec2> randomVerts =
//	{
//		Vec2(00, 00),
//		Vec2(40, 10),
//		Vec2(50, 40),
//		Vec2(100, 20),
//		Vec2(100, 70),
//	};
//
//	Polygon polygon(randomVerts);
//
//	auto trianglesMakingUpPentagon = polygon.triangulate();
//
//	int triangleCount = 0;
//	auto palette = Color::getGrayColorPalette();
//
//	if (randomVerts.size() - 2 > palette.size()) //N - 2 triangles for N vertices (think rectangle)
//	{
//		std::cout << "More triangles in polygon than colors in palette\n";
//	}
//
//
//	std::vector<Vec2> points;
//	std::unordered_map<Vec2, Color> pointsToColors;
//
//	for (const auto& triangle : trianglesMakingUpPentagon)
//	{
//		//auto currentTrianglePoints = triangle.getPointsThatOutlineTriangle(); 
//		auto currentTrianglePoints = triangle.getPointsThatFillTriangle();
//
//		points.insert(points.begin(), currentTrianglePoints.begin(), currentTrianglePoints.end());
//
//		for (const auto& point : currentTrianglePoints)
//		{
//			pointsToColors.insert({ point, Color(palette[triangleCount % palette.size()]) }); //note the mod operator
//		}
//		triangleCount++;
//	}
//
//	GraphicsDemo gd(pointsToColors);
//	gd.draw("filledInTrianglesOfPentagram-grayPalette.png");
//}
//
//void demoLineDrawing()
//{
//	int squareBoxSize = 200;
//
//	int x0 = 20;
//	int y0 = 60;
//	int x1 = 50;
//	int y1 = 50;
//
//	Edge e(Vec2(x0, y0), Vec2(x1, y1));
//
//	auto points = e.getPointsOfLineSegment();
//
//	ImageBMP image(squareBoxSize, squareBoxSize, Color(0, 0, 0));
//
//	std::unordered_map<Vec2, Color> pixelsToColors;
//
//	for (const auto& point : points)
//	{
//		pixelsToColors.insert({ point, ColorEnum::Magenta });
//	}
//
//	image.fillPixelMatrix(pixelsToColors);
//
//	std::string filename = "aLine.png";
//	image.saveAsPNG(filename);
//
//	system(filename.c_str());
//}
//
////void demoLinedSierpinski()
////{
////	GraphicsDemo gd;
////	Edge AB = { Vec2(0, 0), Vec2(1000, 0) };
////
////	Triangle t(AB);
////
////	gd.fillPointsOfSierpinski(t, 5, Color(0, 0, 0));
////
////	std::string filename = "linedSierpinski.png";
////	gd.draw(filename);
////}
//
//void demoFlatBottomAndFlatTopTriangles()
//{
//	//flat bottom example:
//	//Vec2 v0 = { 0, 8 };
//	//Vec2 v1 = { 9, 0 };
//	//Vec2 v2 = { 3, 0 }; //deliberately "disordered" to test the lexicographic sort in Triangle's parameterized constructor def.
//
//	//flat top example:
//	Vec2 v0 = {200, 0};
//	Vec2 v1 = {0, 500}; 
//	Vec2 v2 = {200, 500}; //ordered because I am more or less sure that lexico sort is working
//
//
//	Triangle triangle({ v0, v1, v2 });
//
//	std::vector<Vec2> filledPoints = triangle.getPointsThatFillTriangle();
//
//	/*Makes a border 10 pixels above, below, left, and right of the triangle*/
//	constexpr int PADDING = 10;
//	int width = triangle.getBoundingBoxDimensions().width + PADDING;
//	int height = triangle.getBoundingBoxDimensions().height + PADDING;
//	Color bgrdColor = Color(0, 0, 0);
//
//	ImageBMP image(width, height, bgrdColor);
//
//
//	std::unordered_map<Vec2, Color> pixelsToColors; 
//	int pixelCounter = 0; //using just to alternate pixel colors
//	for (const auto& filledPoint : filledPoints)
//	{
//		Color currentColor;
//		if (pixelCounter % 3 == 0)
//		{
//			currentColor = ColorEnum::Red;
//		}
//
//		else if (pixelCounter % 3 == 1)
//		{
//			currentColor = ColorEnum::Green; //
//		}
//
//		else
//		{
//			currentColor = ColorEnum::Blue;
//		}
//
//		pixelsToColors.insert({ {filledPoint.x, filledPoint.y}, currentColor });
//		pixelCounter++; //
//	}
//
//	image.fillPixelMatrix(pixelsToColors);
//
//	std::string filename = "image.bmp";
//
//	image.saveAsBMP(filename);
//
//	std::system(filename.c_str());
//}
//
//void giveMeAPentagon(const Color& pentagonColor)
//{
//	std::vector<Vec2> regularPentagonVerts =
//	{
//		Vec2(90, 30), //deliberately NOT in CW or CCW order
//		Vec2(60, 50),
//		Vec2(80, 00),
//		Vec2(50, 00),
//		Vec2(40, 30)
//	};
//
//	Polygon polygon(regularPentagonVerts);
//
//	//auto approxCentroid = polygon.getApproximateCentroid(); 
//	auto triangles = polygon.triangulate();
//
//	std::vector<Vec2> fillpoints;
//
//	for (const auto& triangle : triangles)
//	{
//		auto currentFillpoints = triangle.getPointsThatFillTriangle();
//
//		fillpoints.insert(
//			fillpoints.end(), currentFillpoints.begin(), currentFillpoints.end());
//
//
//	}
//
//	GraphicsDemo graphicsDemo(fillpoints, pentagonColor);
//	graphicsDemo.draw("possiblyAPentagon.png");
//}
//
///*NOTE that a pentagram is NOT convex*/
//void giveMeAPentagram(const Color& pentagramColor)
//{
//	std::vector<Vec2> pentagramVerts =
//	{
//		Vec2(50, 100),   // top outer
//		Vec2(65, 65),    // inner right
//		Vec2(100, 65),   // outer right
//		Vec2(70, 45),    // inner bottom-right
//		Vec2(80, 0),     // outer bottom-right
//		Vec2(50, 25),    // inner bottom
//		Vec2(20, 0),     // outer bottom-left
//		Vec2(30, 45),    // inner bottom-left
//		Vec2(0, 65),     // outer left
//		Vec2(35, 65)     // inner left
//	};
//
//	Polygon polygon(pentagramVerts);
//
//	//auto approxCentroid = polygon.getApproximateCentroid(); 
//	auto triangles = polygon.triangulate();
//
//	std::vector<Vec2> fillpoints;
//
//	for (const auto& triangle : triangles)
//	{
//		auto currentFillpoints = triangle.getPointsThatFillTriangle();
//
//		fillpoints.insert(
//			fillpoints.end(), currentFillpoints.begin(), currentFillpoints.end());
//	}
//
//	GraphicsDemo graphicsDemo(fillpoints, pentagramColor);
//	graphicsDemo.draw("possiblyAPentagram.png");
//}