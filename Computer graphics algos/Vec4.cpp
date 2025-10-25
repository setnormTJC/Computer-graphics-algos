#include "Vec4.h"
#include "MyException.h"

Vec4::Vec4(const float x, const float y, const float z, const float w)
	:x(x), y(y), z(z), w(w)
{
}

std::ostream& operator<<(std::ostream& os, const Vec4& v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return os;
}


Vec2 Vec4::xy()
{
	return Vec2(x,y);
}

bool Vec4::operator==(const Vec4& rhs) const
{
	constexpr float NDCepsilon = 1e-4; //tolerance for floating point comparison IF dealing with [-1 to 1] (NDC) values 

	return std::fabs(x - rhs.x) < NDCepsilon &&
		std::fabs(y - rhs.y) < NDCepsilon &&
		std::fabs(z - rhs.z) < NDCepsilon &&
		std::fabs(w - rhs.w) < NDCepsilon;
}



Vec4& Vec4::operator*(const float& rhs) const
{
	Vec4 scalarMultiple = { x * rhs, y * rhs, z * rhs, w * rhs };
	return scalarMultiple;
}

float Vec4::operator*(const Vec4& rhs) const
{
	float dotProduct =
		(x * rhs.x) + (y * rhs.y) + (z * rhs.z) + (w * rhs.w);

	return dotProduct;
}

float Vec4::operator[](const int pos) const
{
	if (pos < 0 || pos > 3) throw MyException("Vec4 pos must be between 0 and 3", __LINE__, __FILE__);
	if (pos == 0) return x;
	else if (pos == 1) return y;
	else if (pos == 2) return z;
	else return w; 
}

float& Vec4::operator[](const int pos)
{
	if (pos < 0 || pos > 3) throw MyException("Vec4 pos must be between 0 and 3", __LINE__, __FILE__);
	if (pos == 0) return x;
	else if (pos == 1) return y;
	else if (pos == 2) return z;
	else return w;
}


/*New guys:*/

Vec4 Vec4::operator-(const Vec4& rhs) const
{
	return { (x - rhs.x), (y - rhs.y), (z - rhs.z), 0.0f };
}

float Vec4::getMagnitude() const
{
	return sqrt(x * x + y * y + z * z); //ignore w 
}

Vec4 Vec4::normalize() const
{
	float magnitude = getMagnitude(); 
	
	return { x / magnitude, y / magnitude, z / magnitude, 0.0f };
}


Vec4 Vec4::cross(const Vec4& rhs) const
{
	//see "Matrix notation" section of: https://en.wikipedia.org/wiki/Cross_product#Computing	
	auto firstTerm = (y * rhs.z) - (z * rhs.y); 
	auto secondTerm = (z * rhs.x) - (x * rhs.z); 
	auto thirdTerm = (x * rhs.y) - (y * rhs.x);

	//NOTE: cross product is defined for 3D vector - not a Vec4 - set w = 0.0f 
	Vec4 crossProduct = { firstTerm, secondTerm, thirdTerm, 0.0f };
	return crossProduct;
}

float Vec4::dot(const Vec4& rhs) const
{
	return x * rhs.x + y * rhs.y + z*rhs.z + 0.0f;
}


