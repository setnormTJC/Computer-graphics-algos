#pragma once

#include<string> 

#include"Vec2.h"

std::string getTimestampForFilename();

/*For use in determining if 2 Edges intersect
@returns 0 -> if "collinear", 1 if "clockwise", 2 if "CCW"*/
int orientation(const Vec2& a, const Vec2& b, const Vec2& c); 

/*Also used for edge intersection stuff*/
bool onSegment(const Vec2& p, const Vec2& q, const Vec2& r);