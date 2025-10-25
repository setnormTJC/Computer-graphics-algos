#include "Camera.h"

Camera::Camera(const int& screenWidth, const int& screenHeight, float fovY)
	:screenWidth(screenWidth), screenHeight(screenHeight), fovY(fovY)
{
	aspectRatio = (float)screenWidth / screenHeight;
	//this->screenHeight = 123; //won't work! screen dims are reference vars!
	constexpr float epsilon = 1e-4;

	if (std::fabs(zNear - 0.0f) < epsilon)
	{
		throw MyException("zNear was within 0.0001 of 0.0", __LINE__, __FILE__);
	}

	if (zNear < 0.0f)
	{
		throw MyException("zNear cannot be < 0", __LINE__, __FILE__);
	}


}

std::vector<Vec2> Camera::projectToScreen(const std::vector<Vec4>& worldSpaceCoords) const
{
	/*vp-> viewprojection*/
	std::vector<Vec4> vpVerts = multiplyByViewProjectionVerts(worldSpaceCoords); 
	
	//perspective divide
	applyPerspectiveDivide(vpVerts);

	std::vector<Vec2> screenSpaceVerts;
	screenSpaceVerts.reserve(worldSpaceCoords.size()); 
	
	for (const auto& vert : vpVerts)
	{
		Vec2 v = ndcToScreen(vert, screenWidth, screenHeight);

		//ensure that no vertex has an x or y value less than 0 or greater than screenWidth
		
		v.x = std::clamp(v.x, 0, screenWidth - 1);
		v.y = std::clamp(v.y, 0, screenHeight - 1);

		//example of clamp's function: 
		// std::clamp(value, min, max)
		//if v.x < min, set v.x to min
		//else if v.x > max, set v.x to max
		//otherwise, no change to v.x
		screenSpaceVerts.push_back(v); 
	}
	
	return screenSpaceVerts;
}

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
	eye.z -= delta; 
}

void Camera::moveBackward(float delta)
{
	eye.z += delta; 
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

#pragma region privates
Vec2 Camera::ndcToScreen(const Vec4& v, int screenWidth, int screenHeight) const
{
	//(vert.x + 1) maps[-1, 1] to [0, 2]
	//multiplying by 0.5f maps[0, 2] tp [0, 1]  "normalized screen coordinates"
	//multiplying by screenWidth maps[0, 1]  to [0, screenWidth]  "pixel coordinates"
		
	int x = static_cast<int>(std::round((v.x + 1.0f)*0.5f * screenWidth)); 
	int y = static_cast<int>(std::round((1.0f - (v.y + 1.0f) * 0.5f) * screenHeight));

	//the 1.0f - ... is used because y = 0 is typically at the TOP of the screen from image formats
	//but y = -1 is the BOTTOM in NDC coordinates 
	
	if (x < 0 || x >= screenWidth || y < 0 || y >= screenHeight)
	{
		std::cout << "Point outside screen: " << x << ", " << y << "\n";
	}

	return Vec2(x, y);
}

void Camera::applyPerspectiveDivide(std::vector<Vec4>& vpVerts) const
{
	for (auto& vert : vpVerts) //not const 
	{
		if (abs(vert.z) < zNear)
		{
			std::cout << vert << " will be clipped\n";
		}

		//"perspective divide" -> THIS is where points further away get "pushed back"
		vert.x /= vert.w;
		vert.y /= vert.w;
		vert.z /= vert.w;
	}
}

std::vector<Vec4> Camera::multiplyByViewProjectionVerts(const std::vector<Vec4>& worldSpaceCoords) const
{
	auto viewMatrix = Mat4::getViewMatrix(eye, target, up);
	//auto view(orCamera)SpaceCoords = viewMatrix * worldSpaceCoords; //just for my understanding of terminology

	auto projectionMatrix = Mat4::getProjectionMatrix(zFar, zNear, fovY, aspectRatio);

	Mat4 viewProjectionProduct = projectionMatrix * viewMatrix; //recall: order must be PVM (not MVP)

	std::vector<Vec4> vpVerts; 

	vpVerts.reserve(worldSpaceCoords.size());

	for (const auto& vert : worldSpaceCoords)
	{
		auto temp = viewProjectionProduct * vert;
		vpVerts.push_back(temp);
	}

	return vpVerts; 
}
#pragma endregion