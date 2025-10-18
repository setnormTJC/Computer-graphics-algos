#include "Vec4.h"
#include "MyException.h"

Vec4::Vec4(const float x, const float y, const float z, const float w)
	:x(x), y(y), z(z), w(w)
{
}



Vec2 Vec4::xy()
{
	return Vec2(x,y);
}

bool Vec4::operator==(const Vec4& rhs)
{
	constexpr float epsilon = 1e-4; //tolerance for floating point comparison

	return std::fabs(x - rhs.x) < epsilon &&
		std::fabs(y - rhs.y) < epsilon &&
		std::fabs(z - rhs.z) < epsilon &&
		std::fabs(w - rhs.w) < epsilon;
}

Vec4& Vec4::operator*(const float& rhs)
{
	Vec4 scalarMultiple = { x * rhs, y * rhs, z * rhs, w * rhs };
	return scalarMultiple;
}

float Vec4::operator*(const Vec4& rhs)
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



std::ostream& operator<<(std::ostream& os, const Vec4& v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")"; 
	return os; 
}
