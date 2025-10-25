#pragma once

#include<functional>

#include<iostream> 

struct Vec2
{
	float x, y;

	Vec2() = default;

	Vec2(float x, float y)
		:x(x), y(y)
	{

	}

	friend std::ostream& operator << (std::ostream& os, const Vec2& v)
	{
		os << "(" << v.x << ", " << v.y << ")";
		return os;
	}

	bool operator == (const Vec2& rhs) const
	{
		return (this->x == rhs.x && this->y == rhs.y);	
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

	Vec2 operator-(const Vec2& rhs) const
	{
		return Vec2(*this) -= rhs;
	}

	Vec2 midpoint(const Vec2& b)
	{
		return Vec2{ (x + b.x) / 2.0f, (y + b.y) / 2.0f };
	}




};




/*For making maps with Vec2 as keytype*/
namespace std
{
	template<>
	struct hash<Vec2>
	{
		size_t operator ()(const Vec2& v) const noexcept
		{
			return std::hash<float>()(v.x) ^ (std::hash<float>()(v.y) << 1);
		}
	};
}


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