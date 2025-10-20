#pragma once
#define _USE_MATH_DEFINES

#include<fstream> 
#include <vector>

#include "Mat4.h"

class Camera
{
private:
	//set the up direction to the typical: (y = 1)
	Vec4 up = { 0.0f, 1.0f, 0.0f, 0.0f };
	//NOTE: w = 1.0f for positions that get multiplied by Mat4s
	//THIS "up" has w = 0.0f (not 1.0f)

	Vec4 eye = { 0.0f, 0.0f, 0.0f, 1.0f }; //z = 0.0f, typically, w must equal 1.0f
	Vec4 target = { 0.0f, 0.0f, -1.0f, 1.0f }; //point the eye in the negative z direction 

	float zFar = 10.0f;
	float zNear = 0.1f;
	float fovY = M_PI / 2;
	float aspectRatio = 1.0f; 

public:
	/*zNear is initialized to 1.0f by default*/
	Camera(float fovY = M_PI / 2, float aspectRatio = 1.0f);
	
	/*NOTE: this function has TWO matrix multiplications on a potentially-large number of vertices
	* It is LIKELY to be one of the most expensive function calls. 
	*/
	std::vector<Vec2> projectToScreen(const std::vector<Vec4>& worldSpaceCoords, int screenWidth, int screenHeight);
	
	/*@brief NOTE! `Camera::projectToScreen` must be called AFTER modifying the eye position to get expected result
	@param newEyePosition -> set z component >= -1 (the default nearZ plane) AND set w = 1.0f
	*/
	void setEyePosition(const Vec4& newEyePosition);

	Vec4 getEyePosition(); 

	void logCameraInfo(const std::string& logFilename);
private: 
	/*Helper called by `projectToScreen`*/
	Vec2 ndcToScreen(const Vec4& v, int screenWidth, int screenHeight);

	void applyPerspectiveDivide(std::vector<Vec4>& vpVerts); 

	std::vector<Vec4> multiplyByViewProjectionVerts(const std::vector<Vec4>& worldSpaceCoords);
};

