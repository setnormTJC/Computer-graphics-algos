#include "Camera.h"

Camera::Camera(float fovY, float aspectRatio)
	:fovY(fovY), aspectRatio(aspectRatio)
{
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

std::vector<Vec2> Camera::projectToScreen(const std::vector<Vec4>& worldSpaceCoords, int screenWidth, int screenHeight)
{
	auto viewMatrix = Mat4::getViewMatrix(eye, target, up);
	//auto view(orCamera)SpaceCoords = viewMatrix * worldSpaceCoords; //just for my understanding of terminology

	auto projectionMatrix = Mat4::getProjectionMatrix(zFar, zNear, fovY, aspectRatio);

	Mat4 viewProjectionProduct = projectionMatrix * viewMatrix; //recall: order must be PVM (not MVP)

	/*vp-> viewprojection*/
	std::vector<Vec4> vpVerts; 
	vpVerts.reserve(worldSpaceCoords.size());

	for (const auto& vert : worldSpaceCoords)
	{
		auto temp = viewProjectionProduct * vert;
		vpVerts.push_back(temp); 
	}
	
	//perspective divide
	for (auto& vert : vpVerts) //not const 
	{
		if (abs(vert.z) < zNear)
		{
			std::cout << vert << " will be clipped\n";
		}
		vert = projectionMatrix * vert;
		//"perspective divide" -> THIS is where points further away get "pushed back"
		vert.x /= vert.w; 
		vert.y /= vert.w;
		vert.z /= vert.w;
	}

	std::vector<Vec2> screenSpaceVerts;
	screenSpaceVerts.reserve(worldSpaceCoords.size()); 
	
	for (const auto& vert : vpVerts)
	{
		Vec2 v = ndcToScreen(vert, screenWidth, screenHeight);
		//if (v.x >= 0 && v.x < screenWidth && v.y >= 0 && v.y < screenHeight)
		//{
			screenSpaceVerts.push_back(v);
		//}
	}
	
	return screenSpaceVerts;
}

void Camera::setEyePosition(const Vec4& newEyePosition)
{
	eye = newEyePosition;
}

Vec4 Camera::getEyePosition()
{
	return eye;
}

void Camera::logCameraInfo(const std::string& logFilename)
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

Vec2 Camera::ndcToScreen(const Vec4& v, int screenWidth, int screenHeight)
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
