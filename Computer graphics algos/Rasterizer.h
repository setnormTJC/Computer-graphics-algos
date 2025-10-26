#pragma once
#include <unordered_map>

#include "Color.h" //for Color 
#include "Mesh.h"
#include "Vec2.h"


/*A "stateless" utility class - has no member vars (similar to a namespace, I suppose)*/
class Rasterizer
{
public: 
	/*@brief Generates a wireframe model by drawing lines between vertices*/
	static std::unordered_map<Vec2, Color> getWireframeLines(const Mesh& mesh, const std::vector<Vec2>& screenVerts,
		const std::vector<Color>& colors);


	//static std::unordered_map<Vec2, Color> getFilledFaces(const Mesh& mesh, const std::vector<Vec2>& screenVerts,
	//	const std::vector<Color>& colors, int screenWidth, int screenHeight);

	/*@brief The "hot path"! Fills the face(s) of triangular faces: complexity O(N * Abar) where N is the number
	* of faces composing the mesh and Abar is the average area of each face
	* @param screenWidth -> NEEDED because this function clamps between 0 and screenWidth
	*/
	static std::unordered_map<Vec2, Color> getFilledFaces(const std::vector<std::array<int, 3>>& frontFaceIndices
		, const std::vector<Vec2>& screenVerts,
		const std::vector<Color>& colors, int screenWidth, int screenHeight);

	/*@brief an overload that uses textures to fill faces*/
	static std::unordered_map<Vec2, Color> getTextureFilledFaces(const std::vector<std::array<int, 3>>& frontFaceIndices
		, const std::vector<Vec2>& screenVerts,
		const std::vector<Vec2>& localUVs, int screenWidth, int screenHeight);

};

