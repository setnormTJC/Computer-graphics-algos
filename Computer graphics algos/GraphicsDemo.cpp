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

void GraphicsDemo::fillPointsOfSierpinski(const Edge& baseEdge, int depth, const Color& color)
{
	if (depth <= 0)
	{
		Triangle tri(baseEdge); 
		auto pts = tri.getPointsThatFillTriangle(); 
		pixels.insert(pixels.end(), pts.begin(), pts.end());
		return; 
	}

	// Construct main triangle
	Triangle tri(baseEdge);

	auto edges = tri.getEdges(); 
	
	// Compute midpoints
	//auto midAB = midpoint(edges[0].v1, );
	auto midAB = edges[0].v1.midpoint(edges[0].v2);
	auto midBC = edges[1].v1.midpoint(edges[1].v2);
	auto midCA = edges[2].v1.midpoint(edges[2].v2);

	// vary color each recursion
	Color c1(255, 0, 0);
	Color c2(0, 255, 255);
	Color c3(0, 0, 255);


	// Recursive calls on the three corner triangles
	fillPointsOfSierpinski(Edge{ edges[1].v1, midAB}, depth - 1, c1);
	fillPointsOfSierpinski(Edge{ midAB, midCA }, depth - 1, c2);
	fillPointsOfSierpinski(Edge{ midCA, edges[1].v1 }, depth - 1, c3);
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

	image.drawFilledTriangle(pixels, Color(255, 0, 255)); 

	//image.writeImageFile(filename); 
	image.saveAsPNG(filename);

	std::system(filename.c_str());
}
