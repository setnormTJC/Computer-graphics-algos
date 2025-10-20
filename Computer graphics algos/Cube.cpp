#include "Cube.h"


#pragma region Cube 
Cube::Cube()
{
	float xLeft = 0.0f; 
	float xRight = +1.0f; 

	float yBottom = 0.0f; 
	float yTop = 1.0f; 

	float zBack = -2.0f; 
	float zFront = zBack + 1.0f; 

	float w = 1.0f; 

	//localCubeVerts =
	//{
	//	{xLeft, yBottom, zBack, w},
	//	{xLeft, yTop, zBack, w},
	//	{xRight, yBottom, zBack, w},
	//	{xRight, yTop, zBack, w},

	//	{xLeft, yBottom, zFront, w},
	//	{xLeft, yTop, zFront, w},
	//	{xRight, yBottom, zFront, w},
	//	{xRight, yTop, zFront, w}
	//};

	//NOTE: this is a tetrahedron - just a temporary change
	localCubeVerts = 
	{
		Vec4(1.0f, 1.0f, 1.0f, 1.0f),
		Vec4(-1.0f, -1.0f, 1.0f, 1.0f),
		Vec4(-1.0f, 1.0f, -1.0f, 1.0f),
		Vec4(1.0f, -1.0f, -1.0f, 1.0f)
	};

	//cubeEdgeIndices =
	//{
	//	{0,1}, {0,2}, {1,3}, {2,3}, // back face
	//	{4,5}, {4,6}, {5,7}, {6,7}, // front face
	//	{0,4}, {1,5}, {2,6}, {3,7}  // connecting edges
	//};

	//tetrahedron vertices - just a temp. change
	cubeEdgeIndices =
	{
		{0,1}, {0,2}, {0,3},
		{1,2}, {1, 3}, {2, 3}
	};

}

Cube::Cube(float xPos, float yPos, float scale, float zOffset)
{
	float xMin = -1.0f * scale + xPos;
	float xMax = 1.0f * scale + xPos;

	float yMin = -1.0f * scale + yPos;
	float yMax = 1.0f * scale + yPos;

	float zMin = -2.0f * scale + zOffset;  // behind front face
	float zMax = -1.0f * scale + zOffset;  // closer face

	float w = 1.0f; 
	//etc.
	//NORMALIZED
	localCubeVerts =
	{
		{xMin, yMin, zMin, w}, //back bottom left
		{xMin, yMax, zMin, w}, //back top left
		{xMax, yMin, zMin, w}, //back bottom right
		{xMax, yMax, zMin, w}, //back top right

		{xMin, yMin, zMax, w}, //front bottom left
		{xMin, yMax, zMax, w}, //front top left
		{xMax, yMin, zMax, w}, //front bottom right
		{xMax, yMax, zMax, w}, //front top right
	};

	cubeEdgeIndices = 
	{
		{0,1}, {0,2}, {1,3}, {2,3}, // back face
		{4,5}, {4,6}, {5,7}, {6,7}, // front face
		{0,4}, {1,5}, {2,6}, {3,7}  // connecting edges
	};

}

std::vector<Vec4> Cube::getLocalCubeVerts()
{
	return localCubeVerts;
}



std::vector<Edge> Cube::getCubeEdges(const std::vector<Vec2>& screenSpaceCubeVerts)
{
	std::vector<Edge> edges; 

	for (auto& [i, j] : cubeEdgeIndices)
	{
		auto a = screenSpaceCubeVerts.at(i);
		auto b = screenSpaceCubeVerts.at(j);
		edges.push_back(Edge(a, b));
	}
	return edges; 
}

std::vector<Vec2> Cube::rasterize(const std::vector<Vec2>& screenVerts)
{
	std::vector<Vec2> rasterPoints; 

	auto edges = getCubeEdges(screenVerts);

	for (const auto& e : edges)
	{
		auto line = e.getPointsOfLineSegment();
		rasterPoints.insert(rasterPoints.end(), line.begin(), line.end());
	}
	return rasterPoints;
}

std::unordered_map<Vec2, Color> Cube::rasterize(const std::vector<Vec2>& screenVerts, 
	const std::vector<Color>& colors)
{
	std::unordered_map<Vec2, Color> positionsToColors; 

	auto edges = getCubeEdges(screenVerts); 

	for (int i = 0; i < edges.size(); ++i)
	{
		auto line = edges[i].getPointsOfLineSegment();

		Color currentColor = colors[i % colors.size()];

		for (const auto& pointOfLine : line)
		{
			positionsToColors.insert({ pointOfLine, currentColor });
		}
	}
	return positionsToColors;
}

#pragma endregion

std::vector<Vec4> Polyhedron::getVerticesFromBlenderObjFile(const std::string& blenderFilename)
{
	return std::vector<Vec4>();
}
