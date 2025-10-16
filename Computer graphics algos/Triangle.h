#pragma once
#include <array>
#include<iostream>
#include<map> 
#include<tuple> //for std::tie
#include <vector>

#include"Vec2.h"

class Edge
{
public: 
	Vec2 v1; 
	Vec2 v2; 

public: 
	Edge() = default; 
	Edge(const Vec2& clientV1, const Vec2& clientV2);	

	bool operator < (const Edge& rhs) const;
	bool operator == (const Edge& rhs) const; 

	float getEdgeLength() const;

	friend std::ostream& operator << (std::ostream& os, const Edge& e);
private: 
	//"adjacent" edges share a vertex 
	bool isAdjacentEdge(const Edge& rhs) const;



	friend class Triangle; //give Triangle access to v1 and v2 
};


class Triangle
{
private:
	std::array<Vec2, 3> vertices{};
	bool isFlatBottom{};
	bool isFlatTop{}; 

	/*Gets filled based on supplied vertices in the constructor*/
	std::array<Edge, 3> edges{}; //bit misleading perhaps?

	/*extrema for scanline algo (and drawing box)*/
	int xMin = INT_MAX; 
	int xMax = INT_MIN; 
	int yMin = INT_MAX;
	int yMax = INT_MIN; 

public: 
	Triangle() = delete;
	Triangle(const std::array<Vec2, 3>& vertices);
	/*construct an EQUILATERAL triangle given one edge*/
	Triangle(const Edge& equilateralEdge);

	std::vector<Vec2> getPointsThatFillTriangle();
	/*@returns */
	Box2D getBoundingBoxDimensions(); 

	float getAngleOfAdjacentEdges(const int indexOfFirstEdge, const int indexOfSecondEdge) const;

	std::array<Edge, 3> getEdges(); 

	std::array<Vec2, 3> getVertices(); 


private: 
	/*modifies the member variable vertices such that v[0].y <= v[1].y <= v[2].y*/
	void sortVertices();
	std::vector<Vec2> getPointsThatFillFlatBottomTriangle(); 
		
	std::vector<Vec2> getPointsThatFillFlatTopTriangle();
	
};


/*Anticipated use: playing with algos that break arbitrary polygons into constituent triangles*/
class Polygon
{
private: 
	std::vector<Triangle> trianglesMakingUpThePolygon;

};