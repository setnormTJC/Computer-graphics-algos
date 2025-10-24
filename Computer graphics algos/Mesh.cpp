#include "Mesh.h"
#include <set>
#include "MyException.h"


//#pragma region Cube 
//Cube::Cube()
//{
//
//	// Cube vertices
//	//localVerts = {
//	//	Vec4(-1.0f, -1.0f, -1.0f, 1.0f), // 0: left-bottom-back
//	//	Vec4(1.0f, -1.0f, -1.0f, 1.0f), // 1: right-bottom-back
//	//	Vec4(1.0f,  1.0f, -1.0f, 1.0f), // 2: right-top-back
//	//	Vec4(-1.0f,  1.0f, -1.0f, 1.0f), // 3: left-top-back
//	//	Vec4(-1.0f, -1.0f,  1.0f, 1.0f), // 4: left-bottom-front
//	//	Vec4(1.0f, -1.0f,  1.0f, 1.0f), // 5: right-bottom-front
//	//	Vec4(1.0f,  1.0f,  1.0f, 1.0f), // 6: right-top-front
//	//	Vec4(-1.0f,  1.0f,  1.0f, 1.0f)  // 7: left-top-front
//	//};
//
//	//// Cube edges (12 edges)
//	//edgeIndices = {
//	//	{0,1}, {1,2}, {2,3}, {3,0}, // back face
//	//	{4,5}, {5,6}, {6,7}, {7,4}, // front face
//	//	{0,4}, {1,5}, {2,6}, {3,7}  // connecting edges
//	//};
//
//	//// Cube faces (6 faces, each as 4 vertices)
//	//faceIndices = {
//	//	{0,1,2,3}, // back
//	//	{4,5,6,7}, // front
//	//	{0,1,5,4}, // bottom
//	//	{2,3,7,6}, // top
//	//	{0,3,7,4}, // left
//	//	{1,2,6,5}  // right
//	//};
//	//NOTE: this is a tetrahedron - just a temporary change
//
//
//}

//#pragma region old Cube stuff
//Cube::Cube(float xPos, float yPos, float scale, float zOffset)
//{
//	float xMin = -1.0f * scale + xPos;
//	float xMax = 1.0f * scale + xPos;
//
//	float yMin = -1.0f * scale + yPos;
//	float yMax = 1.0f * scale + yPos;
//
//	float zMin = -2.0f * scale + zOffset;  // behind front face
//	float zMax = -1.0f * scale + zOffset;  // closer face
//
//	float w = 1.0f; 
//	//etc.
//	//NORMALIZED
//	localVerts =
//	{
//		{xMin, yMin, zMin, w}, //back bottom left
//		{xMin, yMax, zMin, w}, //back top left
//		{xMax, yMin, zMin, w}, //back bottom right
//		{xMax, yMax, zMin, w}, //back top right
//
//		{xMin, yMin, zMax, w}, //front bottom left
//		{xMin, yMax, zMax, w}, //front top left
//		{xMax, yMin, zMax, w}, //front bottom right
//		{xMax, yMax, zMax, w}, //front top right
//	};
//
//	edgeIndices = 
//	{
//		{0,1}, {0,2}, {1,3}, {2,3}, // back face
//		{4,5}, {4,6}, {5,7}, {6,7}, // front face
//		{0,4}, {1,5}, {2,6}, {3,7}  // connecting edges
//	};
//
//}
//
//std::vector<Vec4> Cube::getLocalCubeVerts() const
//{
//	return localVerts;
//}
//
//
//
//std::vector<Edge> Cube::getCubeEdges(const std::vector<Vec2>& screenSpaceCubeVerts)
//{
//	std::vector<Edge> edges; 
//
//	for (auto& [i, j] : edgeIndices)
//	{
//		auto a = screenSpaceCubeVerts.at(i);
//		auto b = screenSpaceCubeVerts.at(j);
//		edges.push_back(Edge(a, b));
//	}
//	return edges; 
//}
//
//std::vector<Triangle> Cube::getFaces(const std::vector<Vec2>& screenSpaceVerts) const
//{
//	std::vector<Triangle> faces; 
//
//	for (const auto& index : faceIndices)
//	{
//		std::array<Vec2, 3> verts =
//		{
//			screenSpaceVerts[index[0]],
//			screenSpaceVerts[index[1]],
//			screenSpaceVerts[index[2]]
//		};
//		faces.push_back(verts);
//	}
//
//	return faces; 
//}
//
//std::vector<Vec2> Cube::rasterize(const std::vector<Vec2>& screenVerts)
//{
//	std::vector<Vec2> rasterPoints; 
//
//	auto edges = getCubeEdges(screenVerts);
//
//	for (const auto& e : edges)
//	{
//		auto line = e.getPointsOfLineSegment();
//		rasterPoints.insert(rasterPoints.end(), line.begin(), line.end());
//	}
//	return rasterPoints;
//}
//
//std::unordered_map<Vec2, Color> Cube::rasterize(const std::vector<Vec2>& screenVerts, 
//	const std::vector<Color>& colors)
//{
//	std::unordered_map<Vec2, Color> positionsToColors; 
//
//	auto edges = getCubeEdges(screenVerts); 
//
//	for (int i = 0; i < edges.size(); ++i)
//	{
//		auto line = edges[i].getPointsOfLineSegment();
//
//		Color currentColor = colors[i % colors.size()];
//
//		for (const auto& pointOfLine : line)
//		{
//			positionsToColors.insert({ pointOfLine, currentColor });
//		}
//	}
//	return positionsToColors;
//}
//
//std::unordered_map<Vec2, Color> Cube::filledRasterize(const std::vector<Vec2>& screenVerts, 
//	const std::vector<Color>& colors) const
//{
//	std::unordered_map<Vec2, Color> positionsToColors;
//
//	auto faces = getFaces(screenVerts); 
//
//	for (int i = 0; i < faces.size(); ++i)
//	{
//		auto points = faces[i].getPointsThatFillTriangle(); 
//		Color currentColor = colors[i % colors.size()];
//
//		for (const auto& point : points)
//		{
//			positionsToColors.insert({ point, currentColor });
//		}
//	}
//
//	return positionsToColors; 
//}
//
//#pragma endregion



Mesh::Mesh(const std::string& blenderObjFilename)
{
	//if filename does not contain .obj, throw
	getDataFromBlenderObjFile(blenderObjFilename);

}

Mesh::Mesh(const CommonPolyhedronType& commonPolyhedronType)
{
	switch (commonPolyhedronType)
	{
	case CommonPolyhedronType::tetrahedron:
		constructTetrahedron(); 
		break; 
	case CommonPolyhedronType::equilateralSquarePyramid:
		constructSquarePyramid(); 
		break;
	case CommonPolyhedronType::triangularPrism: 
		constructTriangularPrism(); 
		break; 

	case CommonPolyhedronType::octahedron: 
		constructOctahedron(); 
		break; 

	case CommonPolyhedronType::cube: 
		constructCube(); 
		break; 
		
	default: 
		throw MyException("Mesh constructor only accepts tetrahedron, pyramid, prism, octahedron, or cube, (or blender obj filename)",
			__LINE__, __FILE__);
	}
}

void Mesh::getDataFromBlenderObjFile(const std::string& blenderFilename)
{
	//Build me up, Buttercup, baby:
	//https://www.youtube.com/watch?v=hSofzQURQDk
}

const std::vector<Vec4>& Mesh::getLocalVertices() const
{
	return localVerts; 
}



std::vector<Triangle> Mesh::getTriangularFaces(const std::vector<Vec2>& screenSpaceVerts) const
{
	std::vector<Triangle> triangularFaces; 

	triangularFaces.reserve(triangularFaceIndices.size());  //prevent frequent reallocation 

	for (const auto& faceIndices : triangularFaceIndices)
	{
		std::array<Vec2, 3> verts =
		{
			screenSpaceVerts[faceIndices[0]],
			screenSpaceVerts[faceIndices[1]],
			screenSpaceVerts[faceIndices[2]]
		};

		triangularFaces.emplace_back(verts);
	}
	return triangularFaces; 
}

#pragma region private Mesh function implementations
void Mesh::constructTetrahedron()
{
	localVerts =
	{
		Vec4(1.0f, 1.0f, 1.0f, 1.0f), //top
		Vec4(-1.0f, -1.0f, 1.0f, 1.0f), //left bottom front
		Vec4(-1.0f, 1.0f, -1.0f, 1.0f),//left top back
		Vec4(1.0f, -1.0f, -1.0f, 1.0f) //right bottom back
	};

	triangularFaceIndices = 
	{
		{0, 1, 2}, // top-left-front triangle
		{0, 1, 3}, // top-left-back triangle
		{0, 2, 3}, // top-right-back triangle
		{1, 2, 3}  // bottom triangle
	};
}

void Mesh::constructSquarePyramid()
{
	localVerts =
	{
		Vec4(0.0f, 0.0f, 0.0f, 1.0f), //0: back left
		Vec4(1.0f, 0.0f, 0.0f, 1.0f), //1: back right
		Vec4(1.0f, 0.0f, 1.0f, 1.0f), //2: front right
		Vec4(0.0f, 0.0f, 1.0f, 1.0f)  //3: front left
	};
	//the y Value of the 5th vertex:
	float apexHeight = 1.0f / sqrt(2.0f); //this is only true for an "unit" square pyramid
	localVerts.push_back(Vec4(0.5f, apexHeight, 0.5f, 1.0f));//4: apex

	triangularFaceIndices =
	{
		{0, 1, 2}, //back right of base
		{0, 2, 3}, //front left of base
		{1, 2, 4}, //right 
		{0, 1, 4}, //back
		{0, 3, 4}, //left
		{2, 3, 4} //front
	};
}

void Mesh::constructTriangularPrism()
{
	localVerts =
	{
		// bottom triangle (y = 0)
		Vec4(0.0f, 0.0f, 0.0f, 1.0f), //0
		Vec4(1.0f, 0.0f, 0.0f, 1.0f), //1
		Vec4(0.5f, 0.0f, std::sqrt(3.0f) / 2.0f, 1.0f), //2

		// top triangle (y = 1)
		Vec4(0.0f, 1.0f, 0.0f, 1.0f), //3
		Vec4(1.0f, 1.0f, 0.0f, 1.0f), //4
		Vec4(0.5f, 1.0f, std::sqrt(3.0f) / 2.0f, 1.0f)  //5
	};

	triangularFaceIndices =
	{
		// bottom
		{0, 1, 2},
		// top
		{3, 5, 4},

		// sides
		{0, 1, 4},
		{0, 4, 3},

		{1, 2, 5},
		{1, 5, 4},

		{2, 0, 3},
		{2, 3, 5}
	};
}

void Mesh::constructOctahedron()
{
	localVerts =
	{
		Vec4(0.0f,  1.0f,  0.0f, 1.0f), //0 top
		Vec4(1.0f,  0.0f,  0.0f, 1.0f), //1 right
		Vec4(0.0f,  0.0f,  1.0f, 1.0f), //2 front
		Vec4(-1.0f,  0.0f,  0.0f, 1.0f), //3 left
		Vec4(0.0f,  0.0f, -1.0f, 1.0f), //4 back
		Vec4(0.0f, -1.0f,  0.0f, 1.0f)  //5 bottom
	};

	triangularFaceIndices =
	{
		// top half
		{0, 1, 2},
		{0, 2, 3},
		{0, 3, 4},
		{0, 4, 1},

		// bottom half
		{5, 2, 1},
		{5, 3, 2},
		{5, 4, 3},
		{5, 1, 4}
	};
}

void Mesh::constructCube()
{
	// 8 vertices of a cube centered at origin, w=1
	localVerts = 
	{
		Vec4(-1.0f, -1.0f, -1.0f, 1.0f), // 0: left-bottom-back
		Vec4(1.0f, -1.0f, -1.0f, 1.0f), // 1: right-bottom-back
		Vec4(1.0f,  1.0f, -1.0f, 1.0f), // 2: right-top-back
		Vec4(-1.0f,  1.0f, -1.0f, 1.0f), // 3: left-top-back
		Vec4(-1.0f, -1.0f,  1.0f, 1.0f), // 4: left-bottom-front
		Vec4(1.0f, -1.0f,  1.0f, 1.0f), // 5: right-bottom-front
		Vec4(1.0f,  1.0f,  1.0f, 1.0f), // 6: right-top-front
		Vec4(-1.0f,  1.0f,  1.0f, 1.0f)  // 7: left-top-front
	};

	// Each face split into two triangles
	triangularFaceIndices = 
	{
		// Back face (-z)
		{0, 1, 2}, {0, 2, 3},
		// Front face (+z)
		{4, 5, 6}, {4, 6, 7},
		// Left face (-x)
		{0, 3, 7}, {0, 7, 4},
		// Right face (+x)
		{1, 5, 6}, {1, 6, 2},
		// Bottom face (-y)
		{0, 1, 5}, {0, 5, 4},
		// Top face (+y)
		{3, 2, 6}, {3, 6, 7}
	};
}

std::vector<std::pair<int, int>> Mesh::getEdgeIndices() const
{
	std::set<std::pair<int, int>> edgeIndices;
	//set enforces uniqueness (convenient for algorithm), convert to std::vector later
	for (const auto& face : triangularFaceIndices)
	{
		int v0 = face[0];
		int v1 = face[1];
		int v2 = face[2];

		auto addEdge = [&](int a, int b)
			{
				if (a > b) std::swap(a, b); //prevents adding, for example, both [0, 1] and [1, 0] as distinct edges
				edgeIndices.emplace(a, b);
				//NOTE the emplace! C++17 is needed here
				//this is preferable syntax to (silly) pre-C++17:
				//edgeIndices.insert({ a, b }); //braces needed
			};
		addEdge(v0, v1); //a, b above
		addEdge(v1, v2); //also a, b
		addEdge(v2, v0);
	}


	//copy (convert) into vector:
	std::vector<std::pair<int, int>> vectorEquivalent(edgeIndices.begin(), edgeIndices.end());
	return vectorEquivalent;
}
#pragma endregion