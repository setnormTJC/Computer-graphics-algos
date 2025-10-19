#pragma once

#include<array>
#include<string> 
#include<vector> 

#include"Vec2.h"
#include"Vec4.h"


namespace Utils
{


	std::string getTimestampForFilename();

	/*For use in determining if 2 Edges intersect
	@returns 0 -> if "collinear", 1 if "clockwise", 2 if "CCW"*/
	int orientation(const Vec2& a, const Vec2& b, const Vec2& c);

	/*Also used for edge intersection stuff*/
	bool onSegment(const Vec2& p, const Vec2& q, const Vec2& r);

	/*@returns first Vec2 contains xMin, yMin
	* second Vec2 contains xMax, yMax
	*/
	std::array<Vec2, 2> getExtrema(const std::vector<Vec2>& verts);

}