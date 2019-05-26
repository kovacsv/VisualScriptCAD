#ifndef MODELER_RAYTRACING_HPP
#define MODELER_RAYTRACING_HPP

#include "IncludeGLM.hpp"
#include "Ray.hpp"
#include "Camera.hpp"

namespace Modeler
{

Geometry::Ray GetScreenRay (const Camera& camera, const glm::dvec2& screenSize, const glm::dvec2& screenPos);

}

#endif
