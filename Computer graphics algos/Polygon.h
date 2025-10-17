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

	/*NO sort! Use "winding formula" and pass vertices in CW or CCW order*/
	Polygon(const std::vector<Vec2>& vertices);


	/*Can be used for imposing CW or CCW ordering*/
	Vec2 getApproximateCentroid(); 

	/*A fancy lad*/
	Vec2 determineCentroid(); 
private: 
	std::vector<Triangle> triangulateConvex() const; 
	std::vector<Triangle> triangulateConcave() const;

	
	int determineCentroidX();
	int determineCentroidY();

	void sortClockwiseOrCCW(); 

	/*Verifies "closed chain" of vertices - called by Polygon constructor, possibly throws*/
	void validatePolygon();

	void sortVertices(); 
};
