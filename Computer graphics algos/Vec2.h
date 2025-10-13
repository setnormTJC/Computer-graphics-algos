#pragma once

struct Vec2
{
	int x, y;

	Vec2() = default;

	Vec2(const int x, const int y)
		:x(x), y(y)
	{

	}

	bool operator == (const Vec2& lhs) const
	{
		return (this->x == lhs.x && this->y == lhs.y);	
	}

	template<typename T> 
	Vec2& operator*=(const T& rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	template<typename T>
	Vec2 operator*(const T& rhs) const
	{
		return Vec2(*this) *= rhs;
	}
	
	Vec2& operator+=(const Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vec2 operator+(const Vec2& rhs) const
	{
		return Vec2(*this) += rhs;
	}

	Vec2& operator-=(const Vec2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Vec2	operator-(const Vec2& rhs) const
	{
		return Vec2(*this) -= rhs;
	}

};

/*Anticipated use: adding some clarity to width and height when drawing images*/
struct Box2D
{
	int width, height;
	Box2D() = default;
	Box2D(const int width, const int height)
		:width(width), height(height)
	{

	}
};