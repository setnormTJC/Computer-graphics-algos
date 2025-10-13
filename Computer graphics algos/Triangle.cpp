#include "Triangle.h"
#include <algorithm>
#include<exception>
#include <stdexcept>
#include <iostream>

#include<tuple> //for std::tie


Triangle::Triangle(const std::array<Vec2, 3>& vertices)
{
	if (std::find_if(vertices.begin(), vertices.end(),
		[](const Vec2& v)
		{
			return (v.x < 0 || v.y < 0);
		}) 
		!= vertices.end()) 
	{
		throw std::runtime_error("triangle vertices cannot contain negative values\n");
	}

	if (vertices[0] == vertices[1] ||
		vertices[1] == vertices[2] ||
		vertices[2] == vertices[0]) 
	{
		throw std::runtime_error("triangle has duplicate vertices");
	}

	if (vertices[0].x == vertices[1].x && vertices[1].x == vertices[2].x
		||
		vertices[0].y == vertices[1].y && vertices[1].y == vertices[2].y)
	{
		throw std::runtime_error("triangle is ... in fact a line segment\n");
	}

	this->vertices = vertices; 

	sortVertices(); 

	/*Determine extrema (for bounding box and scaline algorithm)*/
	for (const auto& v : vertices)
	{
		if (v.x < xMin) xMin = v.x;
		if (v.x > xMax) xMax = v.x;
		if (v.y < yMin) yMin = v.y;
		if (v.y > yMax) yMax = v.y;
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

std::vector<Vec2> Triangle::getPointsThatFillTriangle()
{
	if (isFlatBottom)
	{
		std::cout << "Constructing a flat bottom triangle\n";
		return getPointsThatFillFlatBottomTriangle(); 
	}

	else if (isFlatTop)
	{
		std::cout << "Constructing flat TOP triangle\n";
		return getPointsThatFillFlatTopTriangle(); 
	}

	else //it must be a "general" triangle
	{
		std::vector<Vec2> pointsThatFillGeneralTriangle; 

		std::cout << "Constructing a general triangle (by making a flat top, then a flat bottom):\n";
		
		float alphaSplit =
			static_cast<float>(vertices[1].y - vertices[0].y) / (vertices[2].y - vertices[0].y);

		const Vec2 vi = vertices[0] +  (vertices[2] - vertices[0]) * alphaSplit;

		if (vertices[1].x < vi.x) //major right (longer (longest?) leg is on right
		{
			pointsThatFillGeneralTriangle = getPointsThatFillFlatBottomTriangle(); 

			//tack points that fill flat TOP triangle onto the end: 
			//pointsThatFillGeneralTriangle.insert(pointsThatFillGeneralTriangle.end(), )

			//finish filling me out 
		}

		else //means major left 
		{
			//fill me out 
		}
	}
}

Box2D Triangle::getBoundingBoxDimensions()
{	
	return { xMax - xMin, yMax - yMin };
}


std::vector<Vec2> Triangle::getPointsThatFillFlatBottomTriangle()
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

	constexpr int dy = 1; //not needed, just for my understanding (dy/dx = slope)
	for (int y = yMin; y < yMax; y = y + dy) //careful with edge case here - obviously ...
	{
		for (int x = xLeft; x <= xRight; ++x)
		{
			filledPoints.push_back({ x, y });
		}
		xLeft = xLeft + leftEdgeInverseSlope * dy; //think: xFinal = xInitial +  (dx/dt)dt [t takes the place of 'y' here]
		xRight += rightEdgeInverseSlope; // more concise equivalent to line above
	}

	return filledPoints;
}

std::vector<Vec2> Triangle::getPointsThatFillFlatBottomTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2)
{
	return std::vector<Vec2>();
}

std::vector<Vec2> Triangle::getPointsThatFillFlatTopTriangle()
{
	auto bottom = vertices[0]; 
	auto topLeft = vertices[1]; 
	auto topRight = vertices[2]; 

	std::vector<Vec2> filledPoints; 

	float leftEdgeInverseSlope = static_cast<float>(topLeft.x - bottom.x) / (topLeft.y - bottom.y);
	float rightEdgeInverseSlope = static_cast<float>(topRight.x - bottom.x) / (topRight.y - bottom.y);

	float xLeft = static_cast<float>(bottom.x);
	float xRight = static_cast<float>(bottom.x); 

	for (int y = yMin; y < yMax; ++y) //step up one row of pixels at a time 
	{
		for (int x = xLeft; x <= xRight; ++x) //again, careful with edge cases 
		{
			filledPoints.push_back({ x, y });
		}
		xLeft = xLeft + leftEdgeInverseSlope; 
		xRight += rightEdgeInverseSlope; 
	}

	return filledPoints; 
}

std::vector<Vec2> Triangle::getPointsThatFillFlatTopTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2)
{
	return std::vector<Vec2>();
}
