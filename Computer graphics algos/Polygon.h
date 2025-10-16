#pragma once
#include <vector>
#include "Triangle.h"

class Set
{
	
};

/*Anticipated use: playing with algos that break arbitrary polygons into constituent triangles*/
class Polygon
{
private:
	std::vector<Vec2> vertices; 

public: 
	bool isConvex() const; 
	std::vector<Triangle> triangulate() const;

	Polygon(const std::vector<Vec2>& vertices);
private: 
	std::vector<Triangle> triangulateConvex() const; 
	std::vector<Triangle> triangulateConcave() const;

	/*Verifies "closed chain" of vertices - called by Polygon constructor, possibly throws*/
	void validatePolygon();

	void sortVertices(); 
};
