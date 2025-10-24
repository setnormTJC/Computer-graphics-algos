#pragma once
#include <unordered_map>

#include "ImageBMP.h" //for Color 
#include "Mesh.h"
#include "Vec2.h"

/*A "stateless" utility class - has no member vars (similar to a namespace, I suppose)*/
class Rasterizer
{
public: 
	/*@brief Generates a wireframe model by drawing lines between vertices*/
	static std::unordered_map<Vec2, Color> getWireframeLines(const Mesh& mesh, const std::vector<Vec2>& screenVerts,
		const std::vector<Color>& colors);

	/*@brief Fills the face(s) of triangular faces
	* @param screenWidth -> NEEDED because this function clamps between 0 and screenWidth
	*/
	static std::unordered_map<Vec2, Color> getFilledFaces(const Mesh& mesh, const std::vector<Vec2>& screenVerts,
		const std::vector<Color>& colors, int screenWidth, int screenHeight);
};

