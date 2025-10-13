#pragma once
#include <array>
#include <vector>

#include"Vec2.h"

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
private: 
	/*modifies the member variable vertices such that v[0].y <= v[1].y <= v[2].y*/
	void sortVertices();
	std::vector<Vec2> getPointsThatFillFlatBottomTriangle(); 
	
	/*Overload for working with Chili approach*/
	std::vector<Vec2> getPointsThatFillFlatBottomTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2);
	
	std::vector<Vec2> getPointsThatFillFlatTopTriangle();
	
	/*Overload for working with Chili approach*/
	std::vector<Vec2> getPointsThatFillFlatTopTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2);
};


/*Anticipated use: playing with algos that break arbitrary polygons into constituent triangles*/
class Polygon
{
private: 
	std::vector<Triangle> trianglesMakingUpThePolygon;

};