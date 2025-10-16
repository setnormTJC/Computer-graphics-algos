#include "GraphicsDemo.h"

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


	pixels.insert(pixels.end(), leftFillpoints.begin(), leftFillpoints.end());
	pixels.insert(pixels.end(), rightFillpoints.begin(), rightFillpoints.end());

}

void GraphicsDemo::fillPointsOfEquilateralTriangle(const Edge& edgeOfEquilateralTriangle)
{
	
	Triangle equilateralTriangle(edgeOfEquilateralTriangle);

	auto fillpoints = equilateralTriangle.getPointsThatFillTriangle();

	pixels.insert(pixels.end(), fillpoints.begin(), fillpoints.end());

}

void GraphicsDemo::fillPointsOfSierpinski(const Triangle& tri, int depth, const Color& color)
{
	if (depth == 0)
	{
		auto pts = tri.getPointsThatFillTriangle();
		//for (auto& p : pts) p.color = color;
		pixels.insert(pixels.end(), pts.begin(), pts.end());
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
	Color c1(255, 0, 0);
	Color c2(0, 255, 255);
	Color c3(255, 223, 0);

	// Recursive calls on the three corner triangles
	fillPointsOfSierpinski(t1, depth - 1, c1);
	fillPointsOfSierpinski(t2, depth - 1, c2);
	fillPointsOfSierpinski(t3, depth - 1, c3);
}

void GraphicsDemo::draw(const std::string& filename)
{
	//get dimensions of bounding box
	auto xMax = pixels.at(0).x; 
	auto yMax = pixels.at(0).y; 

	for (const auto& v : pixels)
	{
		if (v.x > xMax) xMax = v.x;
		if (v.y > yMax) yMax = v.y; 
	}

	Box2D dims(xMax, yMax);


	constexpr int PADDING = 50; //for no particular reason ... 
	imageWidth += dims.width + PADDING;
	imageHeight += dims.height + PADDING;

	ImageBMP image(imageWidth, imageHeight, Color(0, 0, 0));

	image.drawFilledTriangle(pixels, Color(0, 215, 255));

	//image.writeImageFile(filename); 
	image.saveAsPNG(filename);

	std::system(filename.c_str());
}
