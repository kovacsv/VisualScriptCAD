#ifndef MODELER_RAYTRACING_HPP
#define MODELER_RAYTRACING_HPP

#include "IncludeGLM.hpp"
#include "Ray.hpp"
#include "RayIntersection.hpp"
#include "Camera.hpp"
#include "Model.hpp"

namespace Modeler
{

class RayModelIntersection
{
public:
	RayModelIntersection (MeshId meshId, unsigned int triangleIndex, const Geometry::RayIntersection& intersection);

	MeshId						meshId;
	unsigned int				triangleIndex;
	Geometry::RayIntersection	intersection;
};

Geometry::Ray						GetScreenRay (const Camera& camera, const glm::dvec2& screenSize, const glm::dvec2& screenPos);
std::vector<RayModelIntersection>	GetRayModelRayIntersections (const Model& model, const Geometry::Ray& ray);

}

#endif
