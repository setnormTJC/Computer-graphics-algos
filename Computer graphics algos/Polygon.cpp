#include "Polygon.h"
#include <algorithm>
#include<tuple>

bool Polygon::isConvex() const
{
	return true;
}

std::vector<Triangle> Polygon::triangulate() const
{
	if (isConvex())
	{
		return triangulateConvex();
	}

	else
	{
		return triangulateConcave();
	}
}

Polygon::Polygon(const std::vector<Vec2>& vertices)
	:vertices(vertices)
{
	if (vertices.size() < 3) throw std::runtime_error("Polygon must have at least 3 verts");


	sortClockwiseOrCCW();
}

std::vector<Triangle> Polygon::triangulateConvex() const
{
	std::vector<Triangle> fanOfTriangles; 

	Vec2 v0 = vertices[0]; 
	for (size_t i = 1; i < vertices.size() - 1; ++i)
	{
		std::array<Vec2, 3> triangleVerts = { v0, vertices[i], vertices[i + 1] };
	
		Triangle currentTriangle(triangleVerts);
		fanOfTriangles.push_back(currentTriangle); //no braces needed with emplace_back
	}
	
	return fanOfTriangles;
}

std::vector<Triangle> Polygon::triangulateConcave() const
{
	return std::vector<Triangle>();
}

int Polygon::determineCentroidX()
{

	return 0;
}

int Polygon::determineCentroidY()
{
	return 0;
}

void Polygon::sortClockwiseOrCCW()
{
	Vec2 center = getApproximateCentroid();

	std::sort(vertices.begin(), vertices.end(),
		[center](const Vec2& a, const Vec2& b)
		{
			return std::atan2(a.y - center.y, a.x - center.x)
				< std::atan2(b.y - center.y, b.x - center.x);
		});
	//don't worry about the details here ... 
	//this imposes CW or CCW 

}

Vec2 Polygon::getApproximateCentroid()
{
	Vec2 approximateCentroid = {0, 0};

	for (int i = 0; i < vertices.size(); ++i)
	{
		approximateCentroid.x += vertices[i].x; 
		approximateCentroid.y += vertices[i].y;
	}

	approximateCentroid.x = approximateCentroid.x / vertices.size();
	approximateCentroid.y = approximateCentroid.y / vertices.size();

	return approximateCentroid;
}

Vec2 Polygon::determineCentroid()
{
	int Cx = determineCentroidX(); 
	int Cy = determineCentroidY(); 

	return Vec2(Cx, Cy);
}



void Polygon::validatePolygon()
{
	if (vertices.size() < 3) throw std::runtime_error("polygon must contain at least 3 verts");

	sortVertices(); 


}

void Polygon::sortVertices()
{
	std::sort(vertices.begin(), vertices.end(),
		[](const Vec2& v1, const Vec2& v2)
		{
			//if a.y == b.y, sort by x values  
			return std::tie(v1.y, v1.x) < std::tie(v2.y, v2.x);
		});
}