#include"Edge.h"

#include <stdexcept>
#include "MyException.h"

#include"Utils.h"

#pragma region Edge
Edge::Edge(const Vec2& clientV1, const Vec2& clientV2)
	:v1(clientV1), v2(clientV2)
{
	if (v1 == v2)
	{
		throw MyException("An edge cannot be made of 2 identical vertices", __LINE__, __FILE__);
	}

	//sort edges by y (make v1 have the lowest y-value)
	if (v1.y > v2.y)
	{
		std::swap(v1, v2);
	}

	//sort by x if y values are equal:
	else if (v1.y == v2.y)
	{
		if (v1.x > v2.x)
		{
			std::swap(v1, v2);
		}
	}
}

bool Edge::operator<(const Edge& rhs) const
{
	return std::tie(v1.x, v1.y, v2.x, v2.y) < std::tie(rhs.v1.x, rhs.v1.y, rhs.v2.x, rhs.v2.y);
	//I THINK this orders by x first. And if x values are equal for v1 and v2, compare y values
}

bool Edge::operator==(const Edge& rhs) const
{
	return (v1 == rhs.v1 && v2 == rhs.v2);
}

float Edge::getEdgeLength() const
{
	return sqrt(
		pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2)
	);
}

std::vector<Vec2> Edge::getPointsOfLineSegmentNaive() const
{
	std::vector<Vec2> points;

	if (v1.x == v2.x) //infinite slope case
	{
		points = getVerticalLineSegmentPoints();
		return points;
	}

	float slope = getSlopeOfLineSegment();

	int xMin = std::min(v1.x, v2.x);
	int xMax = std::max(v1.x, v2.x);
	int yMin = std::min(v1.y, v2.y);
	int yMax = std::max(v1.y, v2.y);

	if (abs(slope) < 1) //step in x direction ... but why?
	{
		//if slope > 1 and you step in the x direction, the points of the line will appear "spaced out"
		for (int x = xMin; x <= xMax; ++x)
		{
			int y = yMin + slope * (x - xMin);
			points.push_back({ x, y });
		}
	}

	else // step in y 
	{
		float inverseSlope = 1 / slope;
		for (int y = yMin; y <= yMax; ++y)
		{
			int x = (y / slope) - (yMin / slope) + xMin;
			points.push_back({ x, y });
		}
	}

	return points;
}


bool Edge::isAdjacentEdge(const Edge& rhs) const
{
	//return (v1 == rhs.v1 || v2 == rhs.v2
	//	||
	//	v2 == rhs.v1 || v1 == rhs.v2);

	return (v2 == rhs.v1 || v1 == rhs.v2);
}

bool Edge::intersects(const Edge& rhs) const
{
	auto o1 = Utils::orientation(v1, v2, rhs.v1);
	auto o2 = Utils::orientation(v1, v2, rhs.v2);
	auto o3 = Utils::orientation(rhs.v1, rhs.v2, v1);
	auto o4 = Utils::orientation(rhs.v1, rhs.v2, v2);

	if (o1 != o2 && o3 != o4)
	{
		return true;
	}
	
	if (o1 == 0 && Utils::onSegment(v1, rhs.v1, v2)) return true;
	if (o2 == 0 && Utils::onSegment(v1, rhs.v2, v2)) return true;
	if (o3 == 0 && Utils::onSegment(rhs.v1, v1, rhs.v2)) return true;
	if (o4 == 0 && Utils::onSegment(rhs.v1, v2, rhs.v2)) return true;

	return false; 
}
std::vector<Vec2> Edge::getPointsOfLineSegment() const
{
	std::vector<Vec2> pts;
	int x0 = static_cast<int>(std::round(v1.x));
	int y0 = static_cast<int>(std::round(v1.y));
	int x1 = static_cast<int>(std::round(v2.x));
	int y1 = static_cast<int>(std::round(v2.y));

	int dx = std::abs(x1 - x0);
	int dy = std::abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (true) {
		pts.push_back({ x0, y0 });
		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x0 += sx; }
		if (e2 < dx) { err += dx; y0 += sy; }
	}
	return pts;
}

float Edge::getSlopeOfLineSegment() const
{
	float slope = static_cast<float>(v2.y - v1.y) / (v2.x - v1.x);

	return slope;
}

std::vector<Vec2> Edge::getVerticalLineSegmentPoints() const
{
	std::vector<Vec2> points;

	//parameterized Edge constructor DOES sort its v1 and v2 args, but just to be safe ...

	if (v1.y >= v2.y)
	{
		throw MyException("Cannot get vertical line segment points - v1.y >= v2.y", __LINE__, __FILE__);
	}

	for (int y = v1.y; y <= v2.y; ++y)
		points.push_back({ v1.x, y });

	return points;
}
#pragma endregion

std::ostream& operator<<(std::ostream& os, const Edge& e)
{
	os << "(" << e.v1.x << "," << e.v1.y << ")"
		<< " -> "
		<< "(" << e.v2.x << "," << e.v2.y << ")";

	return os;
}