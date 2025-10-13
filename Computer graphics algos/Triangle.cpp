#include "Triangle.h"
#include <algorithm>
#include<exception>
#include <stdexcept>
#include <iostream>

#include<tuple> //for std::tie
#include <map>


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

	edges =
	{
		Edge(vertices[0], vertices[1]),
		Edge(vertices[1], vertices[2]),
		Edge(vertices[2], vertices[0])
	};

	/*Determine extrema (for bounding box and scanline algorithm)*/
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


float Triangle::getAngleOfAdjacentEdges(const int indexOfFirstEdge, const int indexOfSecondEdge) const
{	
	if (indexOfFirstEdge < 0 || indexOfFirstEdge > edges.size() - 1
		||
		indexOfSecondEdge < 0 || indexOfSecondEdge > edges.size() - 1)
	{
		throw std::runtime_error("Edge indices must be between 0 and 2");
	}

	//perhaps ASSUME that a user will not pass in two of the same number!

	float a = edges[0].getEdgeLength(); 
	float b = edges[1].getEdgeLength(); 
	float c = edges[2].getEdgeLength(); 

	const float degreeConversionFactor = (180/ 3.14);
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

std::array<Edge, 3> Triangle::getEdges()
{
	return edges;
}


#pragma region Edge
Edge::Edge(const Vec2& clientV1, const Vec2& clientV2)
	:v1(clientV1), v2(clientV2)
{
	if (v1 == v2)
	{
		throw std::runtime_error("An edge cannot be made of 2 identical vertices");
	}

	//sort edges by y (make v1 have the lowest y-value)
	if (v1.y > v2.y)
	{
		std::swap(v1, v2);
	}

	//sort by x if y values are equal:
	else if (v1.y == v2.y)
	{
		if (v1.x > v2.x)
		{
			std::swap(v1, v2);
		}
	}
}

bool Edge::operator<(const Edge& rhs) const
{
	return std::tie(v1.x, v1.y, v2.x, v2.y) < std::tie(rhs.v1.x, rhs.v1.y, rhs.v2.x, rhs.v2.y);
	//I THINK this orders by x first. And if x values are equal for v1 and v2, compare y values
}

bool Edge::operator==(const Edge& rhs) const
{
	return (v1 == rhs.v1 && v2 == rhs.v2); 
}

float Edge::getEdgeLength() const
{
	return sqrt(
		pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2)
	);
}


bool Edge::isAdjacentEdge(const Edge& rhs) const
{
	//if (v1 == rhs.v1 && v2 == rhs.v2)
	//{
	//	throw std::runtime_error("Don't ask if an edge is adjacent to itself");
	//}
	return (v1 == rhs.v1 || v2 == rhs.v2 
		||
		v2 == rhs.v1 || v1 == rhs.v2);
}
	//this SHOULD work without testing, for example, v1 == rhs.v2
	//-because the Edge constructor sorts the vertices 



#pragma endregion

std::ostream& operator<<(std::ostream& os, const Edge& e)
{
	os << "(" << e.v1.x << "," << e.v1.y << ")"
		<< " -> "
		<< "(" << e.v2.x << "," << e.v2.y << ")";

	return os; 
}
