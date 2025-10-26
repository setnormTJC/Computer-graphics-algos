#define _USE_MATH_DEFINES
#include<cmath> 

#include "Triangle.h"
#include <algorithm>
#include<exception>
#include <stdexcept>
#include <iostream>

#include<tuple> //for std::tie
#include <map>
#include "MyException.h"
#include "Mesh.h"


Triangle::Triangle(const std::array<Vec2, 3>& vertices)
{
	if (std::find_if(vertices.begin(), vertices.end(),
		[](const Vec2& v)
		{
			return (v.x < 0 || v.y < 0);
		}) 
		!= vertices.end()) 
	{
		__debugbreak(); 
		throw MyException("triangle vertices cannot contain negative values", __LINE__, __FILE__);
	}

	//...continues by setting this->vertices = vertices then sorting this->vertices for scanline algo
	if (vertices[0] == vertices[1] ||
		vertices[1] == vertices[2] ||
		vertices[2] == vertices[0]) 
	{
		__debugbreak();
		throw MyException("triangle has duplicate vertices", __LINE__, __FILE__);
	}

	if (vertices[0].x == vertices[1].x && vertices[1].x == vertices[2].x
		||
		vertices[0].y == vertices[1].y && vertices[1].y == vertices[2].y)
	{
		__debugbreak();
		throw MyException("triangle is ... in fact a line segment", __LINE__, __FILE__);
	}

	//if (std::abs(getArea()) < 1e-6f)
	//{
	//	throw MyException("Triangle has near-zero area", __LINE__, __FILE__);
	//}

	this->vertices = vertices; 

	sortVertices(); 

	/*Determine extrema (for bounding box and scanline algorithm)*/
	for (const auto& v : vertices)
	{
		if (v.x < xMin) xMin = (int)v.x;
		if (v.x > xMax) xMax = (int)v.x;
		if (v.y < yMin) yMin = (int)v.y;
		if (v.y > yMax) yMax = (int)v.y;
	}

	/*Determine if triangle is flat bottom or flat top or neither*/
	isFlatBottom = this->vertices[0].y == this->vertices[1].y;
	isFlatTop = this->vertices[1].y == this->vertices[2].y;
}



void Triangle::sortVertices()
{
	std::sort(vertices.begin(), vertices.end(),
		[](const Vec2& v1, const Vec2& v2)
		{
			return v1.y < v2.y || (v1.y == v2.y && v1.x < v2.x);
			//NOTE the OR!
			//if a.y == b.y, sort by x values  
			//cleaner alternative:
			//return std::tie(v1.y, v1.x) < std::tie(v2.y, v2.x);
		});
}

std::vector<Vec2> Triangle::getPointsThatFillTriangle(int screenWidth, int screenHeight) const
{
	if (isFlatBottom)
	{
		//std::cout << "Constructing a flat bottom triangle\n";
		return getPointsThatFillFlatBottomTriangle(screenWidth, screenHeight);
	}

	else if (isFlatTop)
	{
		//std::cout << "Constructing flat TOP triangle\n";
		return getPointsThatFillFlatTopTriangle(screenWidth, screenHeight); 
	}

	else //it must be a "general" triangle
	{
		//etc.
		std::vector<Vec2> pointsThatFillGeneralTriangle; 

		//std::cout << "Constructing a general triangle (by making a flat top, then a flat bottom):\n";
		
		//a bit on the logic here: 
		//since v0, v1, and v2 are sorted by ascending y AND this triangle is neither flat top nor flat bottom ...
		//v1's y value will be between (and not equal to) either v0 or v2's y value
		
		//we should split this general triangle into: 
		//tlower = {v0, v1, vNew} //a flat top triangle
		//and 
		//tUpper = {vNew, v1, v2} //a flat bottom triangle
		//and the y-coordinate of vNew will get assigned v1's y value

		float yIntermediate = static_cast<float>(vertices[1].y); //this is ALWAYS the case (regardless of slope)		 		 

		float xIntermediate{}; 

		if (vertices[2].x - vertices[0].x != 0) //guard against div by 0
		{
			//now find intermediate/new vertex's x value based on slope from v0 to v2 
			float slope = static_cast<float>(vertices[2].y - vertices[0].y) / (vertices[2].x - vertices[0].x); //dy/dx
			float b = vertices[0].y - slope * vertices[0].x; //b = y1 - mx1

			xIntermediate = (yIntermediate - b) / slope;
		}

		else //this means the triangle's left side is a vertical line segment 
		{
			//so x0 = xIntermediate = x2
			xIntermediate = static_cast<float>(vertices[2].x); 
		}

		Vec2 intermediateVertex = {xIntermediate, yIntermediate };

		Triangle lower({ vertices[0], vertices[1], intermediateVertex });
		Triangle upper({ intermediateVertex, vertices[1], vertices[2]});

		auto lowerPoints = lower.getPointsThatFillFlatTopTriangle(screenWidth, screenHeight); 
		auto upperPoints = upper.getPointsThatFillFlatBottomTriangle(screenWidth, screenHeight); 

		std::vector<Vec2> filledPoints; 
		filledPoints.reserve(lowerPoints.size() + upperPoints.size());

		filledPoints.insert(filledPoints.end(), lowerPoints.begin(), lowerPoints.end());
		filledPoints.insert(filledPoints.end(), upperPoints.begin(), upperPoints.end()); 
		

		return filledPoints;
	}
}

std::vector<Vec2> Triangle::getPointsThatOutlineTriangle() const
{
	std::vector<Vec2> points; 

	Edge ab = { vertices[0], vertices[1] }; 
	Edge bc = { vertices[1], vertices[2] };
	Edge ca = { vertices[2], vertices[0] };

	auto abPoints = ab.getPointsOfLineSegment(); 
	auto bcPoints = bc.getPointsOfLineSegment();
	auto caPoints = ca.getPointsOfLineSegment();

	points.insert(points.end(), abPoints.begin(), abPoints.end()); 
	points.insert(points.end(), bcPoints.begin(), bcPoints.end());
	points.insert(points.end(), caPoints.begin(), caPoints.end());

	return points;
}

Box2D Triangle::getBoundingBoxDimensions() const
{	
	return { xMax - xMin, yMax - yMin };
}


std::vector<Vec2> Triangle::getPointsThatFillFlatBottomTriangle(int screenWidth, int screenHeight) const
{
	/*Local vars for convenience of visualization:*/
	auto bottomLeft = vertices[0]; 
	auto bottomRight = vertices[1]; 
	auto top = vertices[2]; 

	/*Now to the "meat" of this algo:*/
	std::vector<Vec2> filledPoints;

	float leftEdgeInverseSlope = static_cast<float>(top.x - bottomLeft.x) / (top.y - bottomLeft.y);
	float rightEdgeInverseSlope = static_cast<float>(top.x - bottomRight.x) / (top.y - bottomRight.y);

	/*Need to handle potential infinite slope here ... (ex: typical right triangle with 90 degree angle at origin)*/

	float xLeft = static_cast<float>(bottomLeft.x);
	float xRight = static_cast<float>(bottomRight.x);

	int yStart = static_cast<int>(std::ceil(bottomLeft.y));
	int yEnd = static_cast<int>(std::ceil(top.y)) - 1; // top-left rule: exclude top edge

	for (int y = yStart; y <= yEnd; ++y)
	{
		// Update x endpoints for this scanline
		float xStart = xLeft + leftEdgeInverseSlope * (y - bottomLeft.y);
		float xEnd = xRight + rightEdgeInverseSlope * (y - bottomRight.y);

		int xMin = static_cast<int>(std::ceil(std::min(xStart, xEnd)));
		int xMax = static_cast<int>(std::ceil(std::max(xStart, xEnd))) - 1; // exclude right edge

		xMin = std::clamp(xMin, 0, screenWidth - 1);
		xMax = std::clamp(xMax, 0, screenWidth - 1);

		for (int x = xMin; x <= xMax; ++x)
		{
			int clampedY = std::clamp(y, 0, screenHeight - 1);
			filledPoints.push_back({ static_cast<float>(x), static_cast<float>(clampedY) });
		}
	}

	return filledPoints;
}


std::vector<Vec2> Triangle::getPointsThatFillFlatTopTriangle(int screenWidth, int screenHeight) const
{
	auto bottom = vertices[0];
	auto topLeft = vertices[1];
	auto topRight = vertices[2];

	std::vector<Vec2> filledPoints;

	float leftEdgeInverseSlope = static_cast<float>(topLeft.x - bottom.x) / (topLeft.y - bottom.y);
	float rightEdgeInverseSlope = static_cast<float>(topRight.x - bottom.x) / (topRight.y - bottom.y);

	int yStart = static_cast<int>(std::ceil(bottom.y));
	int yEnd = static_cast<int>(std::ceil(topLeft.y)) - 1; // exclude top edge per top-left rule

	for (int y = yStart; y <= yEnd; ++y)
	{
		float xStart = bottom.x + leftEdgeInverseSlope * (y - bottom.y);
		float xEnd = bottom.x + rightEdgeInverseSlope * (y - bottom.y);

		int xMin = static_cast<int>(std::ceil(std::min(xStart, xEnd)));
		int xMax = static_cast<int>(std::ceil(std::max(xStart, xEnd))) - 1; // exclude right edge

		xMin = std::clamp(xMin, 0, screenWidth - 1);
		xMax = std::clamp(xMax, 0, screenWidth - 1);
		int clampedY = std::clamp(y, 0, screenHeight - 1);

		for (int x = xMin; x <= xMax; ++x)
		{
			filledPoints.push_back({ static_cast<float>(x), static_cast<float>(clampedY) });
		}
	}

	return filledPoints;
}


std::array<Edge, 3> Triangle::getEdges() const
{
	return 	
	{
		Edge(vertices[0], vertices[1]),
		Edge(vertices[1], vertices[2]),
		Edge(vertices[2], vertices[0])
	};

}



std::array<Vec2, 3> Triangle::getVertices() const
{
	return vertices;
}

float Triangle::getArea() const
{
	const Vec2& a = vertices[0]; 
	const Vec2& b = vertices[1]; 
	const Vec2& c = vertices[2]; 

	Vec4 ab((float)(b.x - a.x), (float)(b.y - a.y), 0.0f, 0.0f);
	//Vec4 ac((float)(a.x - c.x), (float)(a.y - c.y), 0.0f, 0.0f); 
	//switched subtraction order above (compared to ab) (magnitude (area) remains the same)
	Vec4 ac((float)(c.x - a.x), (float)(c.y - a.y), 0.0f, 0.0f);

	Vec4 crossProduct = ab.cross(ac); 

	float area = 0.5f * crossProduct.getMagnitude();  

	return area; 
}

std::tuple<float, float, float> Triangle::getBarycentric(const Vec2& point)
{
	Vec2 v0 = vertices[0];
	Vec2 v1 = vertices[1]; 
	Vec2 v2 = vertices[2]; 

	Triangle t0({ v1, v2, point });	
	Triangle t1({ v0, v2, point });
	Triangle t2({ v0, v1, point });

	float areaOfThisTriangle = getArea(); //get area finds cross product of two triangle edges
	float areaOfT0 = t0.getArea(); 
	float areaOfT1 = t1.getArea(); 
	float areaOfT2 = t2.getArea(); 

	float alpha = areaOfT0 / areaOfThisTriangle;
	float beta = areaOfT1 / areaOfThisTriangle;
	float gamma = areaOfT2 / areaOfThisTriangle;


	return {alpha, beta, gamma};
}



#pragma region old triangle funcs
Triangle::Triangle(const Edge& equilateralEdge)
{
	const auto A = equilateralEdge.v1;
	const auto B = equilateralEdge.v2;

	double dx = B.x - A.x;
	double dy = B.y - A.y;

	// rotate AB by +60° around A to get vertex C (be wary of going into -x axis)
	double angle = M_PI / 3.0; // 60 degrees in radians
	double cx = abs(A.x + dx * cos(angle) - dy * sin(angle));
	double cy = abs(A.y + dx * sin(angle) + dy * cos(angle)); //2D rotation matrix here

	// choose the “above-edge” orientation
	vertices[0] = A;
	vertices[1] = B;
	vertices[2] = { (float)cx, (float)cy };

	sortVertices();

	/*Determine extrema (for bounding box and scanline algorithm)*/
	for (const auto& v : vertices)
	{
		if (v.x < xMin) xMin = (int)v.x;
		if (v.x > xMax) xMax = (int)v.x;
		if (v.y < yMin) yMin = (int)v.y;
		if (v.y > yMax) yMax = (int)v.y;
	}

	/*Determine if triangle is flat bottom or flat top or neither*/
	isFlatBottom = this->vertices[0].y == this->vertices[1].y;
	isFlatTop = this->vertices[1].y == this->vertices[2].y;
}



float Triangle::getAngleOfAdjacentEdges(const int indexOfFirstEdge, const int indexOfSecondEdge) const
{
	auto edges = getEdges();

	if (indexOfFirstEdge < 0 || indexOfFirstEdge > (int)edges.size() - 1
		||
		indexOfSecondEdge < 0 || indexOfSecondEdge >(int)edges.size() - 1)
	{
		throw std::runtime_error("Edge indices must be between 0 and 2");
	}

	//perhaps ASSUME that a user will not pass in two of the same number!

	float a = edges[0].getEdgeLength();
	float b = edges[1].getEdgeLength();
	float c = edges[2].getEdgeLength();

	const float degreeConversionFactor = (180 / 3.14);
	//see: https://en.wikipedia.org/wiki/Law_of_cosines
	float abAngle = degreeConversionFactor * acos((c * c - a * a - b * b) / (-2 * a * b)); //gamma
	float bcAngle = degreeConversionFactor * acos((a * a - b * b - c * c) / (-2 * b * c)); //alpha
	float caAngle = degreeConversionFactor * acos((b * b - a * a - c * c) / (-2 * a * c)); //beta

	if (indexOfFirstEdge == 0)
	{
		if (indexOfSecondEdge == 1) return abAngle;
		else return caAngle;
	}

	else if (indexOfFirstEdge == 1)
	{
		if (indexOfSecondEdge == 0) return abAngle;
		else return bcAngle;
	}

	else //indexOfFirstEdge == 2
	{
		if (indexOfSecondEdge == 0) return caAngle;
		else return bcAngle;
	}
}

#pragma endregion
