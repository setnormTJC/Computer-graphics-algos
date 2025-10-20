#pragma once
#include<unordered_map>
#include <vector>

#include "Edge.h"
#include "ImageBMP.h"
#include "Vec4.h"

class Polyhedron
{
protected:
	std::vector<Vec4> vertices;
	std::vector<std::pair<int, int>> edges;
public:
	const std::vector<Vec4>& getVertices() const { return vertices; }
	const std::vector<std::pair<int, int>>& getEdges() const { return edges; }

	std::vector<Vec4> getVerticesFromBlenderObjFile(const std::string& blenderFilename);




};



class Cube : public Polyhedron
{
private: 
	std::vector<Vec4> localCubeVerts;
	/*A la "index buffer"*/
	std::vector<std::pair<int, int>> cubeEdgeIndices;

public: 
	Cube(); 
	/*@brief NOTE: by convention, z = 0 is taken as camera (eye) plane
	@param zOffset -> this calculates "zMax", zMax (AKA: zNear) MUST be > 0*/
	Cube(float xPos, float yPos, float scale, float zOffset);

	std::vector<Vec4> getLocalCubeVerts();
	/*A la "index buffer"*/

	/*Obtains the lines between the vertices*/
	std::vector<Vec2> rasterize(const std::vector<Vec2>& screenVerts);

	std::unordered_map<Vec2, Color> rasterize(const std::vector<Vec2>& screenVerts,
		const std::vector<Color>& colors);

private: 
	std::vector<Edge> getCubeEdges(const std::vector<Vec2>& screenSpaceCubeVerts);
};

class Tetrahedron : public Polyhedron
{

};
