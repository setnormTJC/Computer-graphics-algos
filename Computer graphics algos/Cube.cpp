#include "Cube.h"


#pragma region Cube 
Cube::Cube()
{

	// Cube vertices
	//localVerts = {
	//	Vec4(-1.0f, -1.0f, -1.0f, 1.0f), // 0: left-bottom-back
	//	Vec4(1.0f, -1.0f, -1.0f, 1.0f), // 1: right-bottom-back
	//	Vec4(1.0f,  1.0f, -1.0f, 1.0f), // 2: right-top-back
	//	Vec4(-1.0f,  1.0f, -1.0f, 1.0f), // 3: left-top-back
	//	Vec4(-1.0f, -1.0f,  1.0f, 1.0f), // 4: left-bottom-front
	//	Vec4(1.0f, -1.0f,  1.0f, 1.0f), // 5: right-bottom-front
	//	Vec4(1.0f,  1.0f,  1.0f, 1.0f), // 6: right-top-front
	//	Vec4(-1.0f,  1.0f,  1.0f, 1.0f)  // 7: left-top-front
	//};

	//// Cube edges (12 edges)
	//edgeIndices = {
	//	{0,1}, {1,2}, {2,3}, {3,0}, // back face
	//	{4,5}, {5,6}, {6,7}, {7,4}, // front face
	//	{0,4}, {1,5}, {2,6}, {3,7}  // connecting edges
	//};

	//// Cube faces (6 faces, each as 4 vertices)
	//faceIndices = {
	//	{0,1,2,3}, // back
	//	{4,5,6,7}, // front
	//	{0,1,5,4}, // bottom
	//	{2,3,7,6}, // top
	//	{0,3,7,4}, // left
	//	{1,2,6,5}  // right
	//};
	//NOTE: this is a tetrahedron - just a temporary change
	localVerts = 
	{
		Vec4(1.0f, 1.0f, 1.0f, 1.0f), //top
		Vec4(-1.0f, -1.0f, 1.0f, 1.0f), //left bottom front
		Vec4(-1.0f, 1.0f, -1.0f, 1.0f),//left top back
		Vec4(1.0f, -1.0f, -1.0f, 1.0f) //right bottom back
	};

	//tetrahedron vertices - just a temp. change
	edgeIndices =
	{
		{0,1}, {0,2}, {0,3},
		{1,2}, {1, 3}, {2, 3}
	};

	faceIndices = {
		{0, 1, 2}, // top-left-front triangle
		{0, 1, 3}, // top-left-back triangle
		{0, 2, 3}, // top-right-back triangle
		{1, 2, 3}  // bottom triangle
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
	localVerts =
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

	edgeIndices = 
	{
		{0,1}, {0,2}, {1,3}, {2,3}, // back face
		{4,5}, {4,6}, {5,7}, {6,7}, // front face
		{0,4}, {1,5}, {2,6}, {3,7}  // connecting edges
	};

}

std::vector<Vec4> Cube::getLocalCubeVerts()
{
	return localVerts;
}



std::vector<Edge> Cube::getCubeEdges(const std::vector<Vec2>& screenSpaceCubeVerts)
{
	std::vector<Edge> edges; 

	for (auto& [i, j] : edgeIndices)
	{
		auto a = screenSpaceCubeVerts.at(i);
		auto b = screenSpaceCubeVerts.at(j);
		edges.push_back(Edge(a, b));
	}
	return edges; 
}

std::vector<Triangle> Cube::getFaces(const std::vector<Vec2>& screenSpaceVerts) const
{
	std::vector<Triangle> faces; 

	for (const auto& index : faceIndices)
	{
		std::array<Vec2, 3> verts =
		{
			screenSpaceVerts[index[0]],
			screenSpaceVerts[index[1]],
			screenSpaceVerts[index[2]]
		};
		faces.push_back(verts);
	}

	return faces; 
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

std::unordered_map<Vec2, Color> Cube::filledRasterize(const std::vector<Vec2>& screenVerts, const std::vector<Color>& colors)
{
	std::unordered_map<Vec2, Color> positionsToColors;

	auto faces = getFaces(screenVerts); 

	for (int i = 0; i < faces.size(); ++i)
	{
		auto points = faces[i].getPointsThatFillTriangle(); 
		Color currentColor = colors[i % colors.size()];

		for (const auto& point : points)
		{
			positionsToColors.insert({ point, currentColor });
		}
	}

	return positionsToColors; 
}

#pragma endregion

std::vector<Vec4> Polyhedron::getVerticesFromBlenderObjFile(const std::string& blenderFilename)
{
	return std::vector<Vec4>();
}
