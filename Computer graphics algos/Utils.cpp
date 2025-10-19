#include "Utils.h"
#include <algorithm>
#include "Cube.h"

std::string Utils::getTimestampForFilename()
{
	std::string time = __TIME__;

	std::replace(time.begin(), time.end(), ':', '_');

	return time;
}

int Utils::orientation(const Vec2& a, const Vec2& b, const Vec2& c)
{
	double determinantThingy =
		(b.y - a.y)*(c.x - b.x) - (b.x - a.x)*(c.y - b.y); 

	if (determinantThingy == 0)
	{
		return 0; 
	}

	else if (determinantThingy > 0)
	{
		return 1;
	}

	else
	{
		return 2; 
	}
}

bool Utils::onSegment(const Vec2& p, const Vec2& q, const Vec2& r) 
{
	// Return true if q lies on pr
	return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) 
		&&
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
}

std::array<Vec2, 2> Utils::getExtrema(const std::vector<Vec2>& verts)
{
	{
		int xMin = verts[0].x;
		int xMax = verts[0].x;
		int yMin = verts[0].y;
		int yMax = verts[0].y;

		for (const auto& vert : verts)
		{
			if (vert.x < xMin) xMin = vert.x;
			if (vert.x > xMax) xMax = vert.x;
			if (vert.y < yMin) yMin = vert.y;
			if (vert.y > yMax) yMax = vert.y;
		}

		return { { {xMin, yMin}, {xMax, yMax} } }; //again, silly double braces
	}
}



