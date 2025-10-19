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

	friend std::ostream& operator << (std::ostream& os, const Vec4& v);

	//SWIZZLE! Fo shizzle, my nizzle
	Vec2 xy(); 

	bool operator == (const Vec4& rhs) const;

	//bool operator < ...//maybe not? for now

	/*Scalar multiplication - defined for floats and ints*/
	Vec4& operator * (const float& rhs) const; 

	float getMagnitude() const; 

	/*Dot product*/
	float operator * (const Vec4& rhs) const;

	/*Read-only*/
	float operator[] (const int pos) const;

	/*Write*/
	float& operator[] (const int pos);

	/*NOTE: returns w = 0!*/
	Vec4 operator - (const Vec4& rhs) const;

	Vec4 normalize() const; 

	Vec4 cross(const Vec4& rhs) const; 

	float dot(const Vec4& rhs) const; 
};


