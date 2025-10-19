#pragma once
#define _USE_MATH_DEFINES
#include <vector>

#include"Mat4.h"
#include "Vec4.h"

class CoordinateTransformer
{
private: 
	/*These shoud be between -1 and +1 for x, y, z, with w = +1*/
	std::vector<Vec4> verts; 

	/*Uses Mat4's projection matrix and does perspective divide*/
	void projectVerts(const float zFar, const float zNear, const float fovY);

	/*NDC is -1 to 1*/
	Vec2 ndcToScreen(const Vec4& v, int screenWidth, int screenHeight);

public: 
	CoordinateTransformer() = default; 
	CoordinateTransformer(const std::vector<Vec4>& normalizedVerts); 

	std::vector<Vec2> getScreenSpaceVerts(const float zFar, const float zNear, const int screenWidth, const int screenHeight,
		const float fovY);
};

