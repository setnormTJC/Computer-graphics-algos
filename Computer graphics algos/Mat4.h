#pragma once
#include <array>
#include<iostream>
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

	static Mat4 getProjectionMatrix(const float zFar, const float zNear);
};

