#pragma once
#include <vector>
#include "Vec4.h"
#include "Edge.h"



class Cube //:public Object3D (should probably 
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

	std::vector<Vec4> getCubeVerts();
	/*A la "index buffer"*/

	std::vector<Vec2> rasterize(const std::vector<Vec2>& screenVerts);

private: 
	std::vector<Edge> getCubeEdges(const std::vector<Vec2>& screenSpaceCubeVerts);
};

