#ifndef GEOMETRY_PLANEUTILS_HPP
#define GEOMETRY_PLANEUTILS_HPP

#include "Plane.hpp"
#include "Line.hpp"

#include <vector>

namespace Geometry
{

enum class PointPlanePosition
{
	FrontOfPlane,
	BackOfPlane,
	OnPlane
};

class LinePlaneIntersectionResult
{
public:
	LinePlaneIntersectionResult ();

	bool		found;
	glm::dvec3	position;
};

PointPlanePosition				GetPointPlanePosition (const Plane& plane, const glm::dvec3& point);
LinePlaneIntersectionResult		GetLinePlaneIntersection (const Plane& plane, const Line& line);

}

#endif
