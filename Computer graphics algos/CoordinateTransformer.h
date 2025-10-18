#pragma once
#include <vector>

#include"Mat4.h"
#include "Vec4.h"

class CoordinateTransformer
{
private: 
	/*These shoud be between -1 and +1 for x, y, z, with w = +1*/
	std::vector<Vec4> normalizedVerts; 

public: 
	CoordinateTransformer() = default; 
	CoordinateTransformer(const std::vector<Vec4>& normalizedVerts); 

	std::vector<Vec2> getScreenSpaceVerts(const float zFar, float zNear, const int screenWidth, const int screenHeight); 
};

