#pragma once

#include<algorithm> //edge determination from face vertices uses std::swap
#include<unordered_map>
#include <vector>

#include "Edge.h"
#include"Triangle.h"
#include "Vec4.h"


enum class CommonPolyhedronType 
{ 
	tetrahedron,//4 verts
	equilateralSquarePyramid, //5 verts 
	triangularPrism, //6 verts 
	octahedron, //also 6 verts
	cube, //8 verts
};

enum class CommonPolygonType
{
	isocelesTriangle,
	rectangle,
	pentagon
};

class Mesh
{
private:
	std::vector<Vec4> localVerts;
	std::vector<std::array<int, 3>> triangularFaceIndices; 
	
	
	std::vector<Vec2> localUVs; //for texture mapping
 
public:
	Mesh() = delete; 
	/*Parses vertices and faces from a Blender obj file*/
	Mesh(const std::string& blenderObjFilename);
	/*Constructs a tetrahedron, cube, maybe others in the future*/
	Mesh(const CommonPolyhedronType& commonPolyhedronType); 
	Mesh(const CommonPolygonType& commonPolygonType);

	const std::vector<Vec4>& getLocalVertices() const;


	/*@brief use if filling the faces of Mesh*/
	std::vector<Triangle> getTriangularFaces(const std::vector<Vec2>& screenSpaceVerts) const;

	const const std::vector<std::array<int, 3>> getTriangularFaceIndices() const;

	const std::vector<Vec2> getLocalUVs() const; 

private: 
	/*Parses a Blender .obj file to fill vertices and face indices*/
	void getDataFromBlenderObjFile(const std::string& blenderObjFilename);

	/*runs from [-1, 1] in (x, y, z)*/
	void constructTetrahedron(); 
	void constructSquarePyramid(); 
	void constructTriangularPrism(); 
	void constructOctahedron(); 
	void constructCube();

	/*polygons*/
	void constructIsocelesTriangle();
	void constructRectangle();


	/*@brief use this for wireframe rendering*/
	std::vector<std::pair<int, int>> getEdgeIndices() const;


	/*Required for enforcing winding order (for backface culling)*/
	Vec4 getMeshCentroid() const;


	/*Modifies triangularFaceIndices (for backface culling)*/
	void enforceWindingOrder(); 

};



