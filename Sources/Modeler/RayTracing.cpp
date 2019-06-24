#include "RayTracing.hpp"

#include <algorithm>

namespace Modeler
{

RayModelIntersection::RayModelIntersection (MeshId meshId, unsigned int triangleIndex, const Geometry::RayIntersection& intersection) :
	meshId (meshId),
	triangleIndex (triangleIndex),
	intersection (intersection)
{
}

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

std::vector<RayModelIntersection> GetRayModelRayIntersections (const Model& model, const Geometry::Ray& ray)
{
	std::vector<RayModelIntersection> intersections;
	model.EnumerateMeshes ([&] (MeshId meshId, const MeshRef& meshRef) {
		const MeshGeometry& geometry = model.GetMeshGeometry (meshRef);
		const Geometry::BoundingBox& boundingBox = geometry.GetBoundingBox ();
		const glm::dmat4& transformation = meshRef.GetTransformation ();
		Geometry::BoundingBox transformedBoundingBox = boundingBox.Transform (transformation);

		if (!Geometry::HasRayBoundingBoxIntersection (ray, transformedBoundingBox)) {
			return;
		}

		for (unsigned int triangleIndex = 0; triangleIndex < geometry.TriangleCount (); triangleIndex++) {
			const MeshTriangle& triangle = geometry.GetTriangle (triangleIndex);
			glm::dvec3 v1 = geometry.GetVertex (triangle.v1, transformation);
			glm::dvec3 v2 = geometry.GetVertex (triangle.v2, transformation);
			glm::dvec3 v3 = geometry.GetVertex (triangle.v3, transformation);
			Geometry::RayIntersectionResult result = Geometry::GetRayTriangleIntersection (ray, v1, v2, v3);
			if (result.found) {
				intersections.push_back (RayModelIntersection (meshId, triangleIndex, result.intersection));
			}
		}
	});
	std::sort (intersections.begin (), intersections.end (), [] (const RayModelIntersection& a, const RayModelIntersection& b) {
		return a.intersection.distance < b.intersection.distance;
	});
	return intersections;
}

}
