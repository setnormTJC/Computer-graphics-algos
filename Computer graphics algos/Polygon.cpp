#include "Polygon.h"
#include <algorithm>
#include<tuple>

bool Polygon::isConvex() const
{
	vertices;
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

std::vector<Triangle> Polygon::triangulateConvex() const
{
	return std::vector<Triangle>();
}

std::vector<Triangle> Polygon::triangulateConcave() const
{
	return std::vector<Triangle>();
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