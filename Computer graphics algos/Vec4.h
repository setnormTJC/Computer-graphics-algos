#pragma once

#include<iostream> 
#include "Vec2.h"

class Vec4
{
public: 
	float x, y, z, w; 

public: 
	Vec4() = default; 
	Vec4(const float x, const float y, const float z, const float w);

	//SWIZZLE! Fo shizzle, my nizzle
	Vec2 xy(); 

	bool operator == (const Vec4& rhs);

	//bool operator < ...//maybe not? for now

	/*Scalar multiplication - defined for floats and ints*/
	Vec4& operator * (const float& rhs); 

	/*Dot product*/
	float operator * (const Vec4& rhs);

	/*Read-only*/
	float operator[] (const int pos) const;

	/*Write*/
	float& operator[] (const int pos);

	friend std::ostream& operator << (std::ostream& os, const Vec4& v);
};


