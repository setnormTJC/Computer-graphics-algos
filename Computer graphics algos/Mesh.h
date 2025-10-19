#pragma once
#include "Vec4.h"
#include "Mat4.h"
class Mesh
{
	Vec4 position; 
	Vec4 rotation;
	Vec4 scale;

public: 
	Mat4 getModelMatrix() const; 
};

