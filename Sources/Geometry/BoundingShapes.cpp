#include "BoundingShapes.hpp"
#include "Geometry.hpp"

namespace Geometry
{

const BoundingBox InvalidBoundingBox;
const BoundingSphere InvalidBoundingSphere (glm::dvec3 (0.0));

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

void BoundingBox::EnumerateBoundingPoints (const std::function<void (const glm::dvec3&)>& processor) const
{
	if (!isValid) {
		return;
	}

	processor (glm::dvec3 (min.x, min.y, min.z));
	processor (glm::dvec3 (min.x, max.y, min.z));
	processor (glm::dvec3 (max.x, max.y, min.z));
	processor (glm::dvec3 (max.x, min.y, min.z));
	processor (glm::dvec3 (min.x, min.y, max.z));
	processor (glm::dvec3 (min.x, max.y, max.z));
	processor (glm::dvec3 (max.x, max.y, max.z));
	processor (glm::dvec3 (max.x, min.y, max.z));
}

BoundingBox BoundingBox::Transform (const glm::dmat4& transformation) const
{
	BoundingBox transformed;
	EnumerateBoundingPoints ([&] (const glm::dvec3& point) {
		transformed.AddPoint (glm::dvec3 (transformation * glm::dvec4 (point, 1.0)));
	});
	return transformed;
}

BoundingSphere::BoundingSphere (const glm::dvec3& center) :
	isValid (false),
	center (center),
	radius (0.0)
{
}

BoundingSphere::BoundingSphere (const glm::dvec3& center, double radius) :
	isValid (true),
	center (center),
	radius (radius)
{
}

bool BoundingSphere::IsValid () const
{
	return isValid;
}

void BoundingSphere::AddPoint (const glm::dvec3& point)
{
	isValid = true;
	radius = glm::max (radius, glm::distance (center, point));
}

const glm::dvec3& BoundingSphere::GetCenter () const
{
	return center;
}

double BoundingSphere::GetRadius () const
{
	return radius;
}

}
