#include "RayTracing.hpp"

namespace Modeler
{

Geometry::Ray GetScreenRay (const Camera& camera, const glm::dvec2& screenSize, const glm::dvec2& screenPos)
{
	double screenX = screenPos.x / (screenSize.x * 0.5) - 1.0;
	double screenY = screenPos.y / (screenSize.y * 0.5) - 1.0;

	glm::dmat4 projMatrix = camera.GetProjectionMatrix (screenSize.x, screenSize.y);
	glm::dmat4 viewMatrix = camera.GetViewMatrix ();

	glm::dmat4 invMatrix = glm::inverse (projMatrix * viewMatrix);
	glm::dvec4 screenPosition (screenX, -screenY, 1.0, 1.0);
	glm::dvec4 worldPosition = invMatrix * screenPosition;

	glm::dvec3 rayDirection = glm::normalize (glm::dvec3 (worldPosition));
	return Geometry::Ray (camera.GetEye (), rayDirection);
}

}
