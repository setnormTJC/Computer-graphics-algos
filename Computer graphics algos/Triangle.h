#pragma once
#include <array>
#include <vector>

#include"Vec2.h"

#include<map> 
#include<tuple> //for std::tie

struct Edge
{
	Vec2 v1; 
	Vec2 v2; 

	Edge() = default; 
	Edge(const Vec2& v1, const Vec2& v2)
		:v1(v1), v2(v2)
	{

	}

	bool operator < (const Edge& rhs) const 
	{
		return std::tie(v1.x, v1.y, v2.x, v2.y) < std::tie(rhs.v1.x, rhs.v1.y, rhs.v2.x, rhs.v2.y);
		//I THINK this orders by x first. And if x values are equal for v1 and v2, compare y values
	}
};


class Triangle
{
private:
	std::array<Vec2, 3> vertices{};
	bool isFlatBottom{};
	bool isFlatTop{}; 

	/*extrema for scanline algo (and drawing box)*/
	int xMin = INT_MAX; 
	int xMax = INT_MIN; 
	int yMin = INT_MAX;
	int yMax = INT_MIN; 


public: 
	Triangle() = delete;
	Triangle(const std::array<Vec2, 3>& vertices);


	std::vector<Vec2> getPointsThatFillTriangle();
	/*@returns */
	Box2D getBoundingBoxDimensions(); 

	float getEdgeLength(const Edge& edge) const;

private: 
	/*modifies the member variable vertices such that v[0].y <= v[1].y <= v[2].y*/
	void sortVertices();
	std::vector<Vec2> getPointsThatFillFlatBottomTriangle(); 
	
	/*Overload for working with Chili approach*/
	std::vector<Vec2> getPointsThatFillFlatBottomTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2);
	
	std::vector<Vec2> getPointsThatFillFlatTopTriangle();
	
	/*Overload for working with Chili approach*/
	std::vector<Vec2> getPointsThatFillFlatTopTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2);


	std::map<Edge, float> getMapOfEdgeLengths() const;
	std::map<std::pair<Edge, Edge>, float> getMapOfAngles() const; 
};


/*Anticipated use: playing with algos that break arbitrary polygons into constituent triangles*/
class Polygon
{
private: 
	std::vector<Triangle> trianglesMakingUpThePolygon;

};