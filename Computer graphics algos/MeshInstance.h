#pragma once

#define _USE_MATH_DEFINES 

#include "Vec4.h"
#include "Mat4.h"
#include"MyException.h"

/*A "mesh" is a set of 3D vertices. The mesh's faces are composed of Triangles. 
"MeshInstance" means that a particular set of transformations are applied to that "base" mesh (asset)
*/
class MeshInstance
{
	Vec4 trans; 
	Vec4 rot;
	Vec4 scale;


public: 
	MeshInstance(const Vec4& trans, const Vec4& rot, const Vec4& scale);

	/*@brief multiplies local space vert by the "model" matrix
	* @param worldVerts -> the position of the object in a space (potentially) occupied by other objects. 
	 Note that these are MODIFIED by this method (observe the lack of const)
	*/
	void applyTransformation(const std::vector<Vec4>& localVerts, std::vector<Vec4>& worldVerts) const;

	/*Writes rotation, scaling, and transformation data to a log file*/
	void logTransformationInfo(const std::string& logFilename);
};

