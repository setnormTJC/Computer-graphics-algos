#include "Camera.h"

Camera::Camera(const int& screenWidth, const int& screenHeight, float fovY)
	//:screenWidth(screenWidth), screenHeight(screenHeight), fovY(fovY)
	:fovY(fovY), screenHeight(screenHeight), screenWidth(screenWidth)
{
	aspectRatio = (float)screenWidth / (float)screenHeight;
	//this->screenHeight = 123; //won't work! screen dims are reference vars!
	constexpr float zNearEpsilon = (float) 1e-4;

	if (std::fabs(zNear - 0.0f) < zNearEpsilon)
	{
		throw MyException("zNear was within 0.0001 of 0.0", __LINE__, __FILE__);
	}

	if (zNear < 0.0f)
	{
		throw MyException("zNear cannot be < 0", __LINE__, __FILE__);
	}


}

//std::vector<Vec2> Camera::projectToScreen(const std::vector<Vec4>& worldVerts, const Mesh& mesh) const
//{
//	auto verts = worldVerts;//functions below modify this copy in-place
//
//	applyView(verts);
//	cullBackFaces(verts, mesh);
//
//	applyProjection(verts);
//	//perspective divide
//	applyPerspectiveDivide(verts);
//
//	std::vector<Vec2> screenSpaceVerts;
//	screenSpaceVerts.reserve(verts.size()); 
//	
//	for (const auto& vert : verts)
//	{
//		Vec2 v = ndcToScreen(vert);
//
//		//ensure that no vertex has an x or y value less than 0 or greater than screenWidth
//		
//		v.x = std::clamp(v.x, 0.0f, (float)screenWidth - 1.0f);
//		v.y = std::clamp(v.y, 0.0f, (float)screenHeight - 1.0f);
//
//		//example of clamp's function: 
//		// std::clamp(value, min, max)
//		//if v.x < min, set v.x to min
//		//else if v.x > max, set v.x to max
//		//otherwise, no change to v.x
//		screenSpaceVerts.push_back(v); 
//	}
//	
//	return screenSpaceVerts;
//}

void Camera::setEyePosition(const Vec4& newEyePosition)
{
	eye = newEyePosition;
}

Vec4 Camera::getEyePosition() const
{
	return eye;
}

void Camera::moveForward(float delta)
{
	//put a limit to how much camera can move forward (crossing zNear breaks things): 
	float epsilon = (float)1e-3; 

	float minZ = zNear + epsilon; 
	eye.z = std::max(eye.z - delta, minZ);

	if (eye.z == minZ)
	{
		std::cout << "No moving the camera any farther forwards";
		std::cout << " ... you'll cross zNear and break perspective projection floating point precision\n";
	}
}

void Camera::moveBackward(float delta)
{
	float epsilon = (float)1e-3;
	float maxZ = zFar - epsilon; 
	eye.z = std::min(eye.z + delta, maxZ);

	if (eye.z == maxZ)
		std::cout << "Camera reached maximum depth (far plane)." << std::endl;
}

void Camera::moveLeft(float delta)
{
	eye.x += delta; //PLUS here! (even though moving to the left conventionally decreases x)
}

void Camera::moveRight(float delta)
{
	eye.x -= delta; 
}

void Camera::moveUp(float delta)
{
	eye.y -= delta;  //MINUS here!
}

void Camera::moveDown(float delta)
{
	eye.y += delta; 
}

void Camera::logCameraInfo(const std::string& logFilename) const
{
	std::ofstream fout{ logFilename };
	//fout << Camera::aspectRatio << "\n";
	fout << "Camera eye position: " << Camera::eye << "\n";
	//fout << Camera::fovY << "\n";
	//fout << Camera::target << "\n";
	//fout << Camera::up << "\n";
	//fout << Camera::zFar << "\n";
	//fout << Camera::zNeat

	fout.close(); 
}

std::vector<Vec4> Camera::applyView(const std::vector<Vec4>& worldVerts) const
{
	Mat4 viewMatrix = Mat4::getViewMatrix(eye, target, up);

	std::vector<Vec4> viewVerts; 
	viewVerts.reserve(worldVerts.size()); 

	for (auto& vert : worldVerts) //non-const!
	{
		viewVerts.push_back(viewMatrix * vert);
	}

	return viewVerts; 
}

std::vector<std::array<int, 3>> Camera::getFrontFaceIndices (const std::vector<Vec4>& viewVerts, const Mesh& mesh) const
{
	//1. get surface normals of triangles formed with viewVerts: 
	std::vector<std::array<int, 3>> frontFaceIndices;

	auto triangularFaceIndices = mesh.getTriangularFaceIndices();

	for (const std::array<int, 3>&faceIndices : triangularFaceIndices)
	{
		const Vec4& a = viewVerts[faceIndices[0]];
		const Vec4& b = viewVerts[faceIndices[1]];
		const Vec4& c = viewVerts[faceIndices[2]];

		Vec4 surfaceNormal = (b - a).cross(c - a); //assumes a particular winding order!!

		//Vec4 surfaceNormal = (c - a).cross(b - a); //NOT the correct order - see Mesh::enforceWindingOrder()

		Vec4 viewVector = a - eye; //a can also be b or c 
		float dotProduct = surfaceNormal.dot(viewVector); //order doesn't matter here - aDotb = bDota (commutative)

		if (dotProduct <= 0) //or was it <= 0?
		{
			frontFaceIndices.push_back(faceIndices);
		}

		else
		{
			std::cout << "CULLED the 3 backface vertices (in view coordinates): \n";
			std::cout << a << "\n";
			std::cout << b << "\n";
			std::cout << c << "\n";
		}
	}

	return frontFaceIndices; 
}

std::vector<Vec4>  Camera::applyProjection(const std::vector<Vec4>& viewVerts) const
{
	Mat4 projectionMatrix = Mat4::getProjectionMatrix(zFar, zNear, fovY, aspectRatio);
	std::vector<Vec4> projectedVerts; 

	projectedVerts.reserve(viewVerts.size());
	for (auto& vert : viewVerts)
	{
		projectedVerts.push_back( projectionMatrix * vert);
	}

	return projectedVerts;
}

std::vector<Vec4> Camera::applyPerspectiveDivide(const std::vector<Vec4>& clippedVerts) const
{
	std::vector<Vec4> ndcVerts;
	ndcVerts.reserve(clippedVerts.size()); 

	for (auto& vert : clippedVerts) //not const 
	{
		if (abs(vert.z) < zNear)
		{
			std::cout << vert << " will be clipped\n";
		}

		//"perspective divide" -> THIS is where points further away get "pushed back"
		auto newX = vert.x / vert.w;
		auto newY = vert.y / vert.w;
		auto newZ = vert.z / vert.w;

		ndcVerts.push_back({ newX, newY, newZ, vert.w });//no change to w (still 1.0f)
	}

	return ndcVerts;
}

std::vector<Vec2> Camera::ndcToScreen(const std::vector<Vec4>& ndcVerts) const
{
	//(vert.x + 1) maps[-1, 1] to [0, 2]
	//multiplying by 0.5f maps[0, 2] tp [0, 1]  "normalized screen coordinates"
	//multiplying by screenWidth maps[0, 1]  to [0, screenWidth]  "pixel coordinates"

	std::vector<Vec2> screenVerts; 

	for (const Vec4& ndcVert : ndcVerts)
	{
		int x = static_cast<int>(std::round((ndcVert.x + 1.0f) * 0.5f * screenWidth));
		int y = static_cast<int>(std::round((1.0f - (ndcVert.y + 1.0f) * 0.5f) * screenHeight));

		//the 1.0f - ... is used because y = 0 is typically at the TOP of the screen from image formats
		//but y = -1 is the BOTTOM in NDC coordinates 

		//if (x < 0 || x >= screenWidth || y < 0 || y >= screenHeight)
		//{
		//	std::cout << "Point outside screen: " << x << ", " << y << "\n";
		//}

		x = std::clamp(x, 0, screenWidth - 1);
		y = std::clamp(y, 0, screenHeight - 1);

		Vec2 screenVert((float)x, (float)y);

		screenVerts.push_back(screenVert);
	}

	return screenVerts;
}

//std::vector<Vec4> Camera::multiplyByViewProjectionVerts(const std::vector<Vec4>& worldSpaceCoords) const
//{
//
//
//
//
//
//	std::vector<Vec4> vpVerts; 
//	vpVerts.reserve(viewVerts.size());
//
//	for (const auto& vert : viewVerts)
//	{
//		vpVerts.push_back(projectionMatrix * vert);
//	}
//
//	return vpVerts; 
