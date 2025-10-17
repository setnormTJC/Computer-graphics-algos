#pragma once
#include <vector>
#include "Triangle.h"


/*Anticipated use: playing with algos that break arbitrary polygons into constituent triangles*/
class Polygon
{
private:
	std::vector<Vec2> vertices; 

public: 
	
	bool isConvex() const; 
	/*True if no edge in the polygon intersects another edge*/
	bool isSimple() const; 


	/*Uses "winding formula" and sorts vertices in CW or CCW order (using neat trig)*/
	Polygon(const std::vector<Vec2>& vertices);

	/*Can be used for imposing CW or CCW ordering*/
	Vec2 getApproximateCentroid() const; 

	/*The number of triangles returned SHOULD be the number of vertices - 2 (ex: square is 2 triangles0*/
	std::vector<Triangle> triangulate() const;
private: 
	std::vector<Triangle> triangulateConvex() const; 
	std::vector<Triangle> triangulateConcave() const;

	/*Removes gets an outer hull of vertices that results in a convex polygon
	* does not use all vertices passed in by client
	*/
	std::vector<Triangle> convexHullify() const;

	void sortClockwiseOrCCW(); 

	void sortVertices(); 

	/*NOTE: triangle class also has "getEdges" function*/
	std::vector<Edge> getPolygonEdges() const;  
};
