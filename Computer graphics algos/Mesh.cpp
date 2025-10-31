#include "Mesh.h"
#include <set>
#include "MyException.h"



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

	enforceWindingOrder(); 
	//ALTERS the order of triangularFaceIndices so that back face culling can be done
}

Mesh::Mesh(const CommonPolygonType& commonPolygonType)
{
	switch (commonPolygonType)
	{
	case CommonPolygonType::isocelesTriangle: 
		constructIsocelesTriangle();
		break; 
	case CommonPolygonType::rectangle:
		constructRectangle(); 
		break; 
		//fill in the others later
	default:
		throw MyException("Unsupported polygon type", __LINE__, __FILE__);
	}

	enforceWindingOrder();
	//ALTERS the order of triangularFaceIndices so that back face culling can be done
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
	float screenEpsilon = 0.1f; //! don't make this too tight if dealing with "typical" screen coordinates - ex: (0, 0) to (800, 400)

	std::vector<Triangle> triangularFaces; 

	triangularFaces.reserve(triangularFaceIndices.size());  //prevent frequent reallocation 

	for (const auto& faceIndices : triangularFaceIndices)
	{
		std::array<Vec2, 3> triangleVerts =
		{
			screenSpaceVerts[(size_t)faceIndices[0]],
			screenSpaceVerts[(size_t)faceIndices[1]],
			screenSpaceVerts[(size_t)faceIndices[2]]
		};

		if (triangleVerts[0] == triangleVerts[1] || triangleVerts[1] == triangleVerts[2] || triangleVerts[2] == triangleVerts[0])
		{
			continue; //NICE use of continue, don't add this particular face, move on to the next
		}

		Triangle temp({ triangleVerts[0], triangleVerts[1], triangleVerts[2] }); 

		if (temp.getArea() - 0.0f < screenEpsilon) //area of triangle is "close" to 0 - don't draw it
		{
			continue; 
		}

		//"slightly" faster (but less readable) alternative: 
		//float area2 = (verts[1].x - verts[0].x) * (verts[2].y - verts[0].y) -
		//	(verts[2].x - verts[0].x) * (verts[1].y - verts[0].y);
		//if (area2 == 0.0f)
		//	continue;

		triangularFaces.emplace_back(triangleVerts);
	}
	return triangularFaces; 
}

const std::vector<std::array<int, 3>> Mesh::getTriangularFaceIndices() const
{
	return triangularFaceIndices;
}

const std::vector<Vec2> Mesh::getLocalUVs() const
{
	return localUVs;
}

#pragma region private Mesh function implementations

void Mesh::enforceWindingOrder()
{
	Vec4 meshCentroid = getMeshCentroid(); 

	for (std::array<int, 3>&faceIndices : triangularFaceIndices)
	{
		const Vec4& a = localVerts[faceIndices[0]];
		const Vec4& b = localVerts[faceIndices[1]];
		const Vec4& c = localVerts[faceIndices[2]];

		Vec4 triangleSurfaceNormal = (b - a).cross(c - a);
		//the above is a "candidate winding" - it might need to be "flipped" (by, for example, swap b and c) 

		Vec4 toCentroid = meshCentroid - a; //points from triangle towards the centroid 

		if (triangleSurfaceNormal.dot(toCentroid) > 0) 
		{
			//either swap here or don't - not sure which? 
			std::swap(faceIndices[1], faceIndices[2]);
		}
	}
}

Vec4 Mesh::getMeshCentroid() const
{
	Vec4 approximateCentroid = { 0.0f, 0.0f, 0.0f, 1.0f }; //let w = 1.0f here?

	for (const auto& vert : localVerts)
	{
		approximateCentroid.x += vert.x;
		approximateCentroid.y += vert.y;
		approximateCentroid.z += vert.z;
	}

	approximateCentroid.x = approximateCentroid.x / (float)localVerts.size();
	approximateCentroid.y = approximateCentroid.y / (float)localVerts.size();
	approximateCentroid.z = approximateCentroid.z / (float)localVerts.size();

	return approximateCentroid;
}


void Mesh::constructTetrahedron()
{
	localVerts =
	{
		Vec4(1.0f, 1.0f, 1.0f, 1.0f), //top
		Vec4(-1.0f, -1.0f, 1.0f, 1.0f), //left bottom front
		Vec4(-1.0f, 1.0f, -1.0f, 1.0f),//left top back
		Vec4(1.0f, -1.0f, -1.0f, 1.0f) //right bottom back
	};

	triangularFaceIndices = //not necessarily in a "winding" order!
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
	// Vertices (4 per face, 6 faces = 24)
	localVerts = {
		// Back face (-Z)
		Vec4(-1,-1,-1,1), Vec4(1,-1,-1,1), Vec4(1,1,-1,1), Vec4(-1,1,-1,1),
		// Front face (+Z)
		Vec4(-1,-1,1,1), Vec4(1,-1,1,1), Vec4(1,1,1,1), Vec4(-1,1,1,1),
		// Left face (-X)
		Vec4(-1,-1,-1,1), Vec4(-1,-1,1,1), Vec4(-1,1,1,1), Vec4(-1,1,-1,1),
		// Right face (+X)
		Vec4(1,-1,-1,1), Vec4(1,-1,1,1), Vec4(1,1,1,1), Vec4(1,1,-1,1),
		// Bottom face (-Y)
		Vec4(-1,-1,-1,1), Vec4(1,-1,-1,1), Vec4(1,-1,1,1), Vec4(-1,-1,1,1),
		// Top face (+Y)
		Vec4(-1,1,-1,1), Vec4(1,1,-1,1), Vec4(1,1,1,1), Vec4(-1,1,1,1)
	};

	// UVs (one per vertex, aligned to each face)
	localUVs = {
		// Back face
		Vec2(0,0), Vec2(1,0), Vec2(1,1), Vec2(0,1),
		// Front face
		Vec2(0,0), Vec2(1,0), Vec2(1,1), Vec2(0,1),
		// Left face
		Vec2(0,0), Vec2(1,0), Vec2(1,1), Vec2(0,1),
		// Right face
		Vec2(0,0), Vec2(1,0), Vec2(1,1), Vec2(0,1),
		// Bottom face
		Vec2(0,0), Vec2(1,0), Vec2(1,1), Vec2(0,1),
		// Top face
		Vec2(0,0), Vec2(1,0), Vec2(1,1), Vec2(0,1)
	};

	// Face indices (2 triangles per face)
	triangularFaceIndices = {
		{0,1,2}, {0,2,3},       // back
		{4,5,6}, {4,6,7},       // front
		{8,9,10}, {8,10,11},    // left
		{12,13,14}, {12,14,15}, // right
		{16,17,18}, {16,18,19}, // bottom
		{20,21,22}, {20,22,23}  // top
	};
	//// 8 vertices of a cube centered at origin, w=1
	//localVerts = 
	//{
	//	Vec4(-1.0f, -1.0f, -1.0f, 1.0f), // 0: left-bottom-back
	//	Vec4(1.0f, -1.0f, -1.0f, 1.0f), // 1: right-bottom-back
	//	Vec4(1.0f,  1.0f, -1.0f, 1.0f), // 2: right-top-back
	//	Vec4(-1.0f,  1.0f, -1.0f, 1.0f), // 3: left-top-back
	//	Vec4(-1.0f, -1.0f,  1.0f, 1.0f), // 4: left-bottom-front
	//	Vec4(1.0f, -1.0f,  1.0f, 1.0f), // 5: right-bottom-front
	//	Vec4(1.0f,  1.0f,  1.0f, 1.0f), // 6: right-top-front
	//	Vec4(-1.0f,  1.0f,  1.0f, 1.0f)  // 7: left-top-front
	//};

	//// Each face split into two triangles
	//triangularFaceIndices = 
	//{
	//	// Back face (-z)
	//	{0, 1, 2}, {0, 2, 3},
	//	// Front face (+z)
	//	{4, 5, 6}, {4, 6, 7}, //is indeed the only front face after culling with 0 rotation and 0 translation 
	//	// Left face (-x)
	//	{0, 3, 7}, {0, 7, 4},
	//	// Right face (+x)
	//	{1, 5, 6}, {1, 6, 2},
	//	// Bottom face (-y)
	//	{0, 1, 5}, {0, 5, 4},
	//	// Top face (+y)
	//	{3, 2, 6}, {3, 6, 7}
	//};
}

void Mesh::constructIsocelesTriangle()
{
	localVerts =
	{
		//note the z = -1.0f here: 
		Vec4(-1.0f, -1.0f, -1.0f, 1.0f), //0: bottom left 
		Vec4(1.0f, -1.0f, -1.0f, 1.0f), //1: bottom right
		Vec4(0.0f, 1.0f, -1.0f, 1.0f) //2: top
 	};

	triangularFaceIndices = 
	{
		{ 0, 1, 2 }
	}; //only one of 'em for a single triangle ... 

	
	localUVs =
	{
		Vec2(0,0), //bottom left 
		Vec2(1,0), //bottom right
		Vec2(0.5,1) //top
	};
}

void Mesh::constructRectangle()
{
	localVerts =
	{
		//note the z = -1.0f here: 
		Vec4(-1.0f, 0.0f, -1.0f, 1.0f), //0: bottom left 
		Vec4(2.0f, 0.0f, -1.0f, 1.0f), //1: bottom right (deltaX = 3)
		Vec4(2.0f, 2.0f, -1.0f, 1.0f), //2: top right (deltaY = 2)  

		Vec4(-1.0f, 0.0f, -1.0f, 1.0f), //bottom left (again) 
		Vec4(2.0f, 0.0f, -1.0f, 1.0f), //bottom right (again) 
		Vec4(-1.0f, 2.0f, -1.0f, 1.0f) //top left 

	};

	triangularFaceIndices =
	{
		{0, 1, 2}, 
		{0, 1, 5} //bottom left, bottom right, top left 
	}; //two for a rectangle 


	localUVs =
	{
		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f),
		Vec2(1.0f, 0.67f), 

		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f),
		Vec2(0.0f, 0.67f),
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