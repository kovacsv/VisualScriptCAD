#include "PlaneUtils.hpp"
#include "Geometry.hpp"

#include <array>

namespace Geometry
{

LinePlaneIntersectionResult::LinePlaneIntersectionResult () :
	found (false)
{
}

PointPlanePosition GetPointPlanePosition (const Plane& plane, const glm::dvec3& point)
{
	glm::dvec3 planeNormal = glm::normalize (glm::dvec3 (plane.a, plane.b, plane.c));
	double s = glm::dot (point, planeNormal) + plane.d;
	if (IsGreater (s, 0.0)) {
		return PointPlanePosition::FrontOfPlane;
	} else if (IsLower (s, 0.0)) {
		return PointPlanePosition::BackOfPlane;
	}
	return PointPlanePosition::OnPlane;
}

LinePlaneIntersectionResult GetLinePlaneIntersection (const Plane& plane, const Line& line)
{
	LinePlaneIntersectionResult result;
	result.found = false;

	glm::dvec3 planeNormal = glm::normalize (glm::dvec3 (plane.a, plane.b, plane.c));
	glm::dvec3 lineNormal = glm::normalize (line.direction);

	double denom = glm::dot (lineNormal, glm::dvec3 (plane.a, plane.b, plane.c));
	if (IsEqual (denom, 0.0)) {
		return result;
	}

	double nom = -(glm::dot (line.origin, planeNormal) + plane.d);
	double distance = nom / denom;
	result.found = true;
	result.position = line.origin + lineNormal * distance;
	return result;
}

}
