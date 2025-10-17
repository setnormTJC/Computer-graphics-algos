#include "Utils.h"
#include <algorithm>

std::string getTimestampForFilename()
{
	std::string time = __TIME__;

	std::replace(time.begin(), time.end(), ':', '_');

	return time;
}

int orientation(const Vec2& a, const Vec2& b, const Vec2& c)
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

bool onSegment(const Vec2& p, const Vec2& q, const Vec2& r) 
{
	// Return true if q lies on pr
	return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) 
		&&
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
}
