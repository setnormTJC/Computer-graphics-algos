#pragma once
#define _USE_MATH_DEFINES

#include<algorithm>
#include<fstream> 
#include <vector>

#include "Mat4.h"
#include "Mesh.h"

/*@brief This class is responsible for converting "world space coordinates" (x, y, z, w) 
to screen space coordinates (x, y). 
It does this based the given params:
@param up -> establishs what direction is up for the camera - usually the positive unit vector in the y-direction
@param eye -> the location of the camera. The origin (0, 0, 0, 1) is probably a good choice
@param target  -> what direction is the camera pointing? Often the unit vector in the negative z-direction 
@param zNear -> objects with z-components of their vertices < zNear will get clipped; set to 10.0f by default
@param zFar -> objects with z-components of their vertices > zFar will get clipped; set to 0.1 by default
@param fovY -> MUST be between 0 and PI (see Mat4::getProjectionMatrix for details)
@param aspectRatio -> screenWidth/screenHeight; a cube might look like a rectangular prism if this is incorrect*/
class Camera
{
private:

	Vec4 eye = { 0.0f, 0.0f, 0.0f, 1.0f }; //z = 0.0f, typically, w must equal 1.0f
	Vec4 target = { 0.0f, 0.0f, -1.0f, 1.0f }; //point the eye in the negative z direction 
	//set the up direction to the typical: (y = 1)
	Vec4 up = { 0.0f, 1.0f, 0.0f, 0.0f };
	//NOTE: w = 1.0f for positions that get multiplied by Mat4s
	//THIS "up" has w = 0.0f (not 1.0f)

	float aspectRatio = 1.0f; 
	float fovY = M_PI / 2;
	float zFar = 10.0f;
	float zNear = 0.1f;

	const int& screenHeight;//reference type! (let SDLWrapper OWN these)  - and Camera CANNOT modify this (not that it would want to)
	const int& screenWidth; //reference type! (let SDLWrapper OWN these) 

public:
	/*zNear is initialized to 1.0f by default*/
	Camera(const int& screenWidth, const int& screenHeight, float fovY = M_PI / 2);
	
	/*NOTE: this function has TWO matrix multiplications on a potentially-large number of vertices
	* It is LIKELY to be one of the most expensive function calls. 
	*/
	//std::vector<Vec2> projectToScreen(const std::vector<Vec4>& worldVerts, const Mesh& mesh) const;
	
	/*@brief NOTE! `Camera::projectToScreen` must be called AFTER modifying the eye position to get expected result
	@param newEyePosition -> set z component >= -1 (the default nearZ plane) AND set w = 1.0f
	*/
	void setEyePosition(const Vec4& newEyePosition);

	Vec4 getEyePosition() const; 

	void moveForward(float delta);
	void moveBackward(float delta);

	void moveLeft(float delta);
	void moveRight(float delta);

	void moveUp(float delta); 
	void moveDown(float delta);

	void logCameraInfo(const std::string& logFilename) const;

//private: 
	/*Helpers called by `projectToScreen`*/

	/*Modifies world verts - transforming them (in place, for complexity considerations) to VIEW verts */
	std::vector<Vec4> applyView(const std::vector<Vec4>& worldVerts) const;
	/*Finds normals of all triangular faces, removes vertices whose faces point away from camera (determined by dot product)*/
	std::vector<std::array<int, 3>> getFrontFaceIndices(const std::vector<Vec4>& viewVerts, const Mesh& mesh) const;
	/*puts vertices into "clip space"*/
	std::vector<Vec4>  applyProjection(const std::vector<Vec4>& viewVerts) const;
	/*verts should be in range [-1, 1] after this function finishes*/
	std::vector<Vec4> applyPerspectiveDivide(const std::vector<Vec4>& clippedVerts) const; 
	/**/
	std::vector<Vec2>  ndcToScreen(const std::vector<Vec4>& ndcVerts) const;


};

