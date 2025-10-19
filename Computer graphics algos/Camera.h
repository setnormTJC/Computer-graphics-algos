#pragma once
#include <vector>
#include "Mat4.h"

class Camera
{
private:
	//Vec3 pos;
	//Vec3 target; 

	float fovY;
	float aspectRatio;
	float zNear; 
	float zFar; 


public:
	Camera();

	std::vector<Vec2> projectToScreen(
		const std::vector<Vec4>& worldVerts, int screenWidth, int screenHeight);

	Mat4 getViewMatrix() const;
	Mat4 getProjectionMatrix() const;
};

