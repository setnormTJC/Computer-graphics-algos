#include "GraphicsDemo.h"

#include<tuple>

GraphicsDemo::GraphicsDemo() = default;

GraphicsDemo::GraphicsDemo(const std::vector<Vec2>& points, const Color& colorOfAllPixels)
	:points(points)
{
	fillPointsToColorsMap(colorOfAllPixels); 
}

GraphicsDemo::GraphicsDemo(const std::unordered_map<Vec2, Color>& pointsToColors)
	:pointsToColors(pointsToColors)
{
	fillPoints(); 
}

void GraphicsDemo::fillPointsOfCheckerboard(const int numberOfRows, const int numberOfCols, const int sideLength)
{
	int y = sideLength; 



	for (int row = 0; row < numberOfRows; ++row)
	{
		for (int col = 0; col < numberOfCols; ++col)
		{
			// alternate pattern using parity of row + col
			if ((row + col) % 2 == 0)
			{
				int x = col * sideLength;
				int y = row * sideLength;
				fillPointsOfSquare(x, y, sideLength);
			}
		}
	}


}

void GraphicsDemo::fillPointsOfSquare(const int xPos, const int yPos, const int sideLength)
{
	std::array<Vec2, 3> lowerRightSideTriangleVertices =
	{
		Vec2(xPos, yPos),
		Vec2(xPos + sideLength, yPos),
		Vec2(xPos + sideLength, yPos + sideLength)
	};
	
	std::array<Vec2, 3> upperLeftSideTriangleVertices =
	{
		Vec2(xPos, yPos),
		Vec2(xPos, yPos + sideLength),
		Vec2(xPos + sideLength, yPos + sideLength),
	};

	Triangle lowerRightTriangle(lowerRightSideTriangleVertices);
	Triangle upperLeftTriangle(upperLeftSideTriangleVertices);

	auto leftFillpoints = upperLeftTriangle.getPointsThatFillTriangle(); 
	auto rightFillpoints = lowerRightTriangle.getPointsThatFillTriangle(); 


	points.insert(points.end(), leftFillpoints.begin(), leftFillpoints.end());
	points.insert(points.end(), rightFillpoints.begin(), rightFillpoints.end());

}

void GraphicsDemo::fillPointsOfEquilateralTriangle(const Edge& edgeOfEquilateralTriangle)
{
	
	Triangle equilateralTriangle(edgeOfEquilateralTriangle);

	auto fillpoints = equilateralTriangle.getPointsThatFillTriangle();

	points.insert(points.end(), fillpoints.begin(), fillpoints.end());

}

void GraphicsDemo::fillPointsOfSierpinski(const Triangle& tri, int depth, const Color& color/*, int& triangleCount*/)
{
	if (depth == 0)
	{
		//auto pts = tri.getPointsThatFillTriangle();
		auto pts = tri.getPointsThatOutlineTriangle();

		points.insert(points.end(), pts.begin(), pts.end());

		for (const auto& pixel : points)
		{
			pointsToColors.insert({ pixel, color }); //color varies between c1 - c3, as defined below
		}

		//triangleCount++; 
		return;
	}

	auto edges = tri.getEdges();

	// Compute midpoints
	auto midAB = edges[0].v1.midpoint(edges[0].v2);
	auto midBC = edges[1].v1.midpoint(edges[1].v2);
	auto midCA = edges[2].v1.midpoint(edges[2].v2);

	auto verts = tri.getVertices(); 

	// form the three corner triangles
	Triangle t1({ verts[0], midAB, midCA});
	Triangle t2({ midAB, verts[1], midBC});
	Triangle t3({midCA, midBC, verts[2]});

	// vary color each recursion
	Color c1(ColorEnum::Cyan);
	Color c2(ColorEnum::Magenta);
	Color c3(ColorEnum::White);

	// Recursive calls on the three corner triangles
	fillPointsOfSierpinski(t1, depth - 1, c1/*, triangleCount*/); //left subtriangle
	fillPointsOfSierpinski(t2, depth - 1, c2/*, triangleCount*/); //right subtriangle
	fillPointsOfSierpinski(t3, depth - 1, c3/*, triangleCount*/); //top subtriangle
}



void GraphicsDemo::draw(const std::string& filename)
{
	//get dimensions of bounding box
	auto xMax = points.at(0).x;  
	auto yMax = points.at(0).y; 

	for (const auto& v : points)
	{
		if (v.x > xMax) xMax = v.x;
		if (v.y > yMax) yMax = v.y; 
	}

	Box2D dims(xMax, yMax);


	constexpr int PADDING = 1; //PADDING OF 1 to handle floating point error and off by one errors ... hopefully 
	imageWidth += dims.width + PADDING;
	imageHeight += dims.height + PADDING;

	Color bgrdColor(0, 0, 0);

	ImageBMP image(imageWidth, imageHeight, Color(0, 0, 0));

	//image.drawFilledTriangle(pixels, Color(0, 215, 255));
	image.fillPixelMatrix(pointsToColors);

	image.saveAsPNG(filename);

	std::system(filename.c_str());
}

void GraphicsDemo::fillPointsToColorsMap(const Color& colorOfAllPixels)
{
	for (const auto& pixel : points)
	{
		pointsToColors.insert({ pixel, colorOfAllPixels });
	}
}

void GraphicsDemo::fillPoints()
{
	for (const auto& [point, color] : pointsToColors)
	{
		points.push_back(point); 
	}
}
