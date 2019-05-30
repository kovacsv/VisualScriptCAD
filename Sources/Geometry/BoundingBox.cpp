#include "BoundingBox.hpp"

namespace Geometry
{

BoundingBox::BoundingBox (const glm::dvec3& min, const glm::dvec3& max) :
	min (min),
	max (max)
{

}

}
