#pragma once

#include<iostream> 
#include<vector>

#include "Vec2.h"

class Edge
{
public:
	Vec2 v1;
	Vec2 v2;

public:
	Edge() = default;
	/*Note: this constructor will impose the following ordering on V1 and V2:
	* if v1.y > v2.y, swap
	* else if v1.y == v2.y AND v1.x > v2.x, swap
	*/
	Edge(const Vec2& clientV1, const Vec2& clientV2);

	bool operator < (const Edge& rhs) const;
	bool operator == (const Edge& rhs) const;

	float getEdgeLength() const;

	std::vector<Vec2> getPointsOfLineSegment() const;

	//"adjacent" edges share a vertex 
	bool isAdjacentEdge(const Edge& rhs) const;

	bool intersects(const Edge& rhs) const;

	friend std::ostream& operator << (std::ostream& os, const Edge& e);


private:

	float getSlopeOfLineSegment() const;


	std::vector<Vec2> getVerticalLineSegmentPoints() const;
};