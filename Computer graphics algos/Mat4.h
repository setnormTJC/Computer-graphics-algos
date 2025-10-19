#pragma once
#include <array>
#include<iostream>

#include"MyException.h"
#include "Vec4.h"


class Mat4
{ 	
	std::array<std::array<float, 4>,4> elements{};

public: 
	/*Make identity, by default*/
	Mat4(); 
	void set(const int row, const int column, float value); 
	friend std::ostream& operator << (std::ostream& os, const Mat4& m);

	void operator = (const std::array<std::array<float, 4>, 4>& clientElements);

	/*Matrix multiplication*/
	Mat4 operator* (const Mat4& rhs) const; 

	/*Multiply a matrix by a scalar*/
	Mat4 operator* (const float rhs) const; 
	
	/*Multiply a vector by a matrix*/
	Vec4 operator* (const Vec4& rhs) const; 

	/*@brief no fovX needed - taken care of by fovY calc
	@param aspectRatio -> screenWidth/screenHeight; I default this to 1.0f for now (simple square screens)
	* @param fovY  -> set to M_PI/2 usually; example use: zooming in should DECREASE this value (to a minimum of 0.0 - lest the object FLIPS!)
	*/
	static Mat4 getProjectionMatrix(const float zFar, const float zNear, float fovY, float aspectRatio = 1.0f);
	
	/*@brief The "view" matrix essentially controls where the camera points
	* @param eye -> the position of the camera; ex: {0, 0, 0, 1}
	* @param target -> the spot to look at; ex: {0, 0, -1, 1}
	* @param up -> which way is up for the camera; conventionally = {0, 1, 0, 0} (positive y -axis is up)
	* @returns the simple IDENTITY matrix for the above inputs*/
	static Mat4 getViewMatrix(const Vec4& eye, const Vec4& target, const Vec4& up);
	
	/**
 * @brief Generates a standard right-handed perspective projection matrix.
 * @param zFar  Positive distance from the camera to the far clipping plane.
 * @param zNear Positive distance from the camera to the near clipping plane.
 *              Must be greater than 0 and less than zFar.
 *
 * @return Mat4 A 4x4 projection matrix suitable for transforming vertices from camera
 *              space to clip space.
 *
 * @note
 * - Vertices with z < -zFar or z > -zNear will be clipped.
 * - The matrix assumes a right-handed coordinate system with the camera looking along -Z.
 */
	static Mat4 getProjectionMatrix(const float zFar, const float zNear);
};

