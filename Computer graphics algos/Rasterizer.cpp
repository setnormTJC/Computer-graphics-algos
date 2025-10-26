#include "Rasterizer.h"
#include "Texture.h"
#include "MyException.h"


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

std::unordered_map<Vec2, Color> Rasterizer::getTextureFilledFaces(const std::vector<std::array<int, 3>>& frontFaceIndices, 
	const std::vector<Vec2>& screenVerts, const std::vector<Vec2>& localUVs, int screenWidth, int screenHeight)
{
	//Texture texture("bmpOutputs/checkerboard.bmp");//this is just a temporary (expensive) approach
					//pass this as YET ANOTHER arg or possibly make a member variable of Rasterizer...


	Texture texture("textures/catFace.bmp");//this is just a temporary (expensive) approach

	std::unordered_map<Vec2, Color> positionsToColors;

	positionsToColors.reserve(screenVerts.size());  //NOTE that this (almost certainly) will NOT be enough

	for (size_t i = 0; i < frontFaceIndices.size(); ++i)
	{
		const auto& faceIndices = frontFaceIndices[i];

		Vec2 v0 = screenVerts[faceIndices[0]];
		Vec2 v1 = screenVerts[faceIndices[1]];
		Vec2 v2 = screenVerts[faceIndices[2]];

		Triangle currentFace({ v0, v1, v2 });

		Vec2 uv0 = localUVs[faceIndices[0]]; 
		Vec2 uv1 = localUVs[faceIndices[1]];
		Vec2 uv2 = localUVs[faceIndices[2]];

		std::vector<Vec2> points = currentFace.getPointsThatFillTriangle(screenWidth, screenHeight);

		for (const Vec2& point : points)
		{

			auto [alpha, beta, gamma] = currentFace.getBarycentric(point);
			float sanityCheckSum = alpha + beta + gamma;

			if (std::abs(sanityCheckSum - 1.0f) > 1e-3)
				throw MyException("sum of alpha, beta, gamma delta should be within 1e-3 of 1", __LINE__, __FILE__);

			Vec2 uv = uv0 * alpha + uv1 * beta + uv2 * gamma;
			//NOTE: uv.x and uv.y should be between 0 and 1 -> may break otherwise ?
			//if (uv.x < 0.0f || uv.x > 1.0f || )

			Color texColor = texture.sample(uv); //u and v typically range from [0, 1]

			positionsToColors[point] = texColor; 
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
