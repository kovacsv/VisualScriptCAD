#include "BoundingBox.hpp"
#include "Geometry.hpp"

namespace Geometry
{

BoundingBox::BoundingBox () :
	isValid (false),
	min (INF),
	max (-INF)
{

}

BoundingBox::BoundingBox (const glm::dvec3& min, const glm::dvec3& max) :
	isValid (true),
	min (min),
	max (max)
{

}

bool BoundingBox::IsValid () const
{
	return isValid;
}

void BoundingBox::AddPoint (const glm::dvec3& point)
{
	isValid = true;
	min = glm::min (point, min);
	max = glm::max (point, max);
}

const glm::dvec3& BoundingBox::GetMin () const
{
	return min;
}

const glm::dvec3& BoundingBox::GetMax () const
{
	return max;
}

glm::dvec3 BoundingBox::GetCenter () const
{
	return (min + max) / 2.0;
}

}
