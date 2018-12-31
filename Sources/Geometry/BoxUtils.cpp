#include "BoxUtils.hpp"
#include "Geometry.hpp"

namespace Geometry
{

bool AreBoxesOverlap (const Box& a, const Box& b)
{
	return
		IsGreater (a.max.x ,b.min.x) &&
		IsLower (a.min.x, b.max.x) &&
		IsGreater (a.max.y, b.min.y) &&
		IsLower (a.min.y, b.max.y) &&
		IsGreater (a.max.z, b.min.z) &&
		IsLower (a.min.z, b.max.z);
}

}
