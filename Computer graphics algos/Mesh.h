#pragma once

#include<algorithm> //edge determination from face vertices uses std::swap
#include<unordered_map>
#include <vector>

#include "Edge.h"
#include "ImageBMP.h"
#include"Triangle.h"
#include "Vec4.h"


enum class CommonPolyhedronType 
{ 
	tetrahedron,
	cube
};

class Mesh
{
private:
	std::vector<Vec4> localVerts;
	//std::vector<std::pair<int, int>> edgeIndices;
	std::vector<std::array<int, 3>> triangularFaceIndices; 

public:
	Mesh() = delete; 
	/*Anticipate the default constructor reading from a Blender obj file*/
	Mesh(const std::string& blenderObjFilename);
	/*Constructs a tetrahedron, cube, maybe others in the future*/
	Mesh(const CommonPolyhedronType& commonPolyhedronType); 
	
	const std::vector<Vec4>& getLocalVertices() const;


	/*@brief use if filling the faces of Mesh*/
	std::vector<Triangle> getTriangularFaces(const std::vector<Vec2>& screenSpaceVerts) const;

private: 
	/*Parses a Blender .obj file to fill vertices and face indices*/
	void getDataFromBlenderObjFile(const std::string& blenderObjFilename);

	/*runs from [-1, 1] in (x, y, z)*/
	void constructTetrahedron(); 

	void constructCube();

	/*@brief use this for wireframe rendering*/
	std::vector<std::pair<int, int>> getEdgeIndices() const;

};



//class Cube : public Mesh
//{
//	//...to be continued
//private: 
//	std::vector<Vec4> localVerts;
//	/*A la "index buffer"*/
//	std::vector<std::pair<int, int>> edgeIndices;
//
//	/*NOTE: this is only for a tetrahedron (which has 3 faces) at the moment*/
//	std::vector<std::array<int, 3>> faceIndices; 
//
//public: 
//	Cube(); 
//	/*@brief NOTE: by convention, z = 0 is taken as camera (eye) plane
//	@param zOffset -> this calculates "zMax", zMax (AKA: zNear) MUST be > 0*/
//	Cube(float xPos, float yPos, float scale, float zOffset);
//
//	std::vector<Vec4> getLocalCubeVerts() const;
//	/*A la "index buffer"*/
//
//	/*Obtains the lines between the vertices (color is determined later by client)*/
//	std::vector<Vec2> rasterize(const std::vector<Vec2>& screenVerts);
//
//	/*Obtains the lines between the vertices and applies particular colors*/
//	std::unordered_map<Vec2, Color> rasterize(const std::vector<Vec2>& screenVerts,
//		const std::vector<Color>& colors);
//
//	/*Requires the vertices composing a face of the polyhedron to be known/stored*/
//	std::unordered_map<Vec2, Color> filledRasterize(const std::vector<Vec2>& screenVerts,
//		const std::vector<Color>& colors) const;
//
//private: 
//	std::vector<Edge> getCubeEdges(const std::vector<Vec2>& screenSpaceCubeVerts);
//
//	/*ONLY works for tetrahedron at the moment*/
//	std::vector<Triangle> getFaces(const std::vector<Vec2>& screenSpaceVerts) const;
//};
//
//class Tetrahedron : public Mesh
//{
//
//};
