#pragma once

#define _USE_MATH_DEFINES 

#include "Vec4.h"
#include "Mat4.h"
#include"MyException.h"

/*Class name was not my idea - not the most clear*/
class Mesh
{
	Vec4 trans; 
	Vec4 rot;
	Vec4 scale;

	std::vector<Vec4> localSpaceVerts;

public: 
	Mesh(const std::vector<Vec4>& localSpaceVerts, const Vec4& trans, const Vec4& rot, const Vec4& scale);

	/*multiplies local space vert by the model matrix*/
	std::vector<Vec4> applyModelMatrix() const; 

	/*Writes rotation, scaling, and transformation data to a log file*/
	void logTransformationInfo(const std::string& logFilename);
};

