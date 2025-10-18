#include "CoordinateTransformer.h"

CoordinateTransformer::CoordinateTransformer(const std::vector<Vec4>& normalizedVerts)
	:normalizedVerts(normalizedVerts)
{
}

std::vector<Vec2> CoordinateTransformer::getScreenSpaceVerts(const float zFar, float zNear, const int screenWidth, const int screenHeight)
{
	auto projectionMatrix = Mat4::getProjectionMatrix(zFar, zNear);

	for (auto& vert : normalizedVerts) //not const 
	{
		vert = projectionMatrix * vert;
		vert.x /= vert.w;
		vert.y /= vert.w;
		vert.z /= vert.w;
	}

	/*What gets passed to ImageBMP constructor:*/
	std::vector<Vec2> screenSpaceCubeVerts;


	for (const auto& vert : normalizedVerts)
	{
		Vec2 v;
		v.x = (int)((vert.x + 1.0f) * 0.5f * screenWidth);
		v.y = (int)((1.0f - (vert.y + 1.0f) * 0.5f) * screenHeight); // flip Y

		screenSpaceCubeVerts.push_back(v);
	}

	return screenSpaceCubeVerts;
}
