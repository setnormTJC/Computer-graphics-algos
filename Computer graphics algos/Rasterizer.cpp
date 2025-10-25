#include "Rasterizer.h"


std::unordered_map<Vec2, Color> Rasterizer::getWireframeLines(const Mesh& mesh, const std::vector<Vec2>& screenVerts,
	const std::vector<Color>& colors)
{
	std::unordered_map<Vec2, Color> positionsToColors;

	//auto edgeIndices = mesh.getEdgeIndices(); 



	//auto edges = poly.getCubeEdges(screenVerts);

	//for (int i = 0; i < edges.size(); ++i)
	//{
	//	auto line = edges[i].getPointsOfLineSegment();

	//	Color currentColor = colors[i % colors.size()];

	//	for (const auto& pointOfLine : line)
	//	{
	//		positionsToColors.insert({ pointOfLine, currentColor });
	//	}
	//}
	return positionsToColors;
}

std::unordered_map<Vec2, Color> Rasterizer::getFilledFaces(const std::vector<std::array<int, 3>>& frontFaceIndices, 
	const std::vector<Vec2>& screenVerts, const std::vector<Color>& colors, int screenWidth, int screenHeight)
{
	std::unordered_map<Vec2, Color> positionsToColors;


	positionsToColors.reserve(screenVerts.size()); 

	for (size_t i = 0; i < frontFaceIndices.size(); ++i)
	{
		const auto& faceIndices = frontFaceIndices[i]; 
		
		auto v0 = screenVerts[faceIndices[0]];
		auto v1 = screenVerts[faceIndices[1]];
		auto v2 = screenVerts[faceIndices[2]];

		Triangle currentFace({ v0, v1, v2 });

		std::vector<Vec2> points = currentFace.getPointsThatFillTriangle(screenWidth, screenHeight); 
		Color currentColor = colors[i % colors.size()]; //if many faces and fewer colors, don't go out of color bounds 
		
		for (const Vec2& point : points)
		{
			positionsToColors[point] = currentColor;
		}
	}

	return positionsToColors;
}

//std::unordered_map<Vec2, Color> Rasterizer::getFilledFaces(const Mesh& mesh, const std::vector<Vec2>& screenVerts,
//	const std::vector<Color>& colors, int screenWidth, int screenHeight)
//{
//	std::unordered_map<Vec2, Color> positionsToColors;
//
//	auto faces = mesh.getTriangularFaces(screenVerts); 
//
//	for (size_t i = 0; i < faces.size(); ++i)
//	{
//		auto points = faces[i].getPointsThatFillTriangle(screenWidth, screenHeight);
//		Color currentColor = colors[i % colors.size()]; //if many faces and fewer colors, don't go out of color bounds 
//
//		for (const auto& point : points)
//		{
//			positionsToColors.insert({ point, currentColor });
//		}
//	}
//
//	return positionsToColors;
//}
