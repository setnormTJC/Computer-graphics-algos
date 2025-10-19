#pragma once
#include <vector>
#include "Vec4.h"
#include "Edge.h"
class Cube
{
private: 
	std::vector<Vec4> normalizedCubeVerts;
	/*A la "index buffer"*/
	std::vector<std::pair<int, int>> cubeEdgeIndices;

public: 
	Cube(); 

	std::vector<Vec4> getCubeVerts();
	/*A la "index buffer"*/

	std::vector<Vec2> rasterize(const std::vector<Vec2>& screenVerts);

private: 
	std::vector<Edge> getCubeEdges(const std::vector<Vec2>& screenSpaceCubeVerts);
};

