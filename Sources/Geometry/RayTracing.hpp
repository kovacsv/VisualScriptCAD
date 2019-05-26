#ifndef GEOMETRY_RAYTRACING_HPP
#define GEOMETRY_RAYTRACING_HPP

#include "IncludeGLM.hpp"
#include "Ray.hpp"

namespace Geometry
{

class RayIntersection
{
public:
	RayIntersection ();
	RayIntersection (const glm::dvec3& position, double distance);

	bool		found;
	glm::dvec3	position;
	double		distance;
};

extern const RayIntersection NoIntersection;

RayIntersection		GetRayTriangleIntersection (const Ray& ray, const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3);

}

#endif
