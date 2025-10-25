#include "Polygon.h"
#include <algorithm>
#include<tuple>
#include "MyException.h"

Polygon::Polygon(const std::vector<Vec2>& vertices)
	:vertices(vertices)
{
	if (vertices.size() < 3)
	{
		throw MyException("Polygon must have at least 3 verts", __LINE__, __FILE__);
	}

	sortClockwiseOrCCW();
}

bool Polygon::isConvex() const
{
	//auto n = vertices.size(); 

	//for (size_t i = 0; i < n; ++i)
	//{
	//	const Vec2& a = vertices[i]; 
	//	const Vec2& b = vertices[(i + 1) % n];
	//	const Vec2& c = vertices[(i + 2) % n];
	//}

	return true; //for now
}

bool Polygon::isSimple() const
{
	//auto edges = getPolygonEdges(); 


	//for (int left = 0; left < edges.size(); ++left)
	//{
	//	Edge leftEdge = edges[left]; 

	//	for (int right = left + 1; right < edges.size(); ++right)
	//	{
	//		Edge rightEdge = edges[right]; 

	//		if (!leftEdge.isAdjacentEdge(rightEdge))
	//		{
	//			if (leftEdge.intersects(rightEdge))
	//			{
	//				std::cout << leftEdge << " intersects " << rightEdge << "\n";
	//				return false; 
	//			}
	//		}
	//	}
	//}

	return true; 
}

std::vector<Triangle> Polygon::triangulate() const
{
	if (!isSimple())
	{
		return convexHullify(); //NOTE: at least one (maybe more?) vertex will get ignored
	}

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

std::vector<Triangle> Polygon::convexHullify() const
{
	return std::vector<Triangle>();
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

Vec2 Polygon::getApproximateCentroid() const
{
	Vec2 approximateCentroid = {0, 0};

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		approximateCentroid.x += vertices[i].x; 
		approximateCentroid.y += vertices[i].y;
	}

	approximateCentroid.x = approximateCentroid.x / (int)vertices.size();
	approximateCentroid.y = approximateCentroid.y / (int)vertices.size();

	return approximateCentroid;
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

std::vector<Edge> Polygon::getPolygonEdges() const
{
	std::vector<Edge> edges; 
	
	for (size_t i = 0; i < vertices.size() - 1; ++i)
	{
		edges.push_back({ vertices[i], vertices[i + 1] });
	}
	//close the "chain": 
	edges.push_back({ vertices[vertices.size() - 1], vertices[0] });

	return edges;
}
