#include "MeshGenerators.hpp"
#include "Geometry.hpp"

namespace Modeler
{

#define PI glm::pi<double> ()

static void AddRectangle (Mesh& mesh, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4, MaterialId mat)
{
	mesh.AddTriangle (v1, v2, v3, mat);
	mesh.AddTriangle (v1, v3, v4, mat);
}

static void AddCenteredConvexPolygon (Mesh& mesh, unsigned int centerVertex, const std::vector<unsigned int>& vertices, const glm::dvec3& normal, MaterialId mat)
{
	unsigned int normalIndex = mesh.AddNormal (normal);
	size_t vertexCount = vertices.size ();
	for (size_t i = 0; i < vertexCount; i++) {
		bool last = (i == vertexCount - 1);
		unsigned int a = centerVertex;
		unsigned int b = vertices[i];
		unsigned int c = vertices[last ? 0 : i + 1];
		mesh.AddTriangle (a, b, c, normalIndex, normalIndex, normalIndex, mat);
	}
}

static void GenerateCircularPoints (const glm::dvec3& center, double radius, unsigned int segmentation, const std::function<void (const glm::dvec3& vertex)>& processor)
{
	double segmentAngle = (2.0 * PI) / double (segmentation);
	glm::dmat4 rotMatrix = glm::rotate (glm::dmat4 (1.0), segmentAngle, glm::dvec3 (0.0, 0.0, 1.0));
	glm::dvec3 point (radius, 0.0, 0.0);
	for (unsigned int i = 0; i < segmentation; i++) {
		processor (center + point);
		point = rotMatrix * glm::dvec4 (point, 1.0);
	}
}

static std::vector<unsigned int> AddCircularVertices (Mesh& mesh, const glm::dvec3& center, double radius, unsigned int segmentation)
{
	std::vector<unsigned int> vertices;
	GenerateCircularPoints (center, radius, segmentation, [&] (const glm::dvec3& vertex) {
		vertices.push_back (mesh.AddVertex (vertex));
	});
	return vertices;
}

static glm::dvec3 SphericalToCartesian (double radius, double theta, double phi)
{
	return glm::dvec3 (
		radius * sin (theta) * cos (phi),
		radius * sin (theta) * sin (phi),
		radius * cos (theta)
	);
};

Mesh GenerateBox (const Material& material, const glm::dmat4& transformation, double xSize, double ySize, double zSize)
{
	Mesh mesh;
	MaterialId materialId = mesh.AddMaterial (material);
	mesh.SetTransformation (transformation);

	double x = xSize / 2.0;
	double y = ySize / 2.0;
	double z = zSize / 2.0;

	mesh.AddVertex (-x, -y, -z);
	mesh.AddVertex (x, -y, -z);
	mesh.AddVertex (x, -y, z);
	mesh.AddVertex (-x, -y, z);
	mesh.AddVertex (-x, y, -z);
	mesh.AddVertex (x, y, -z);
	mesh.AddVertex (x, y, z);
	mesh.AddVertex (-x, y, z);

	AddRectangle (mesh, 0, 1, 2, 3, materialId);
	AddRectangle (mesh, 1, 5, 6, 2, materialId);
	AddRectangle (mesh, 5, 4, 7, 6, materialId);
	AddRectangle (mesh, 4, 0, 3, 7, materialId);
	AddRectangle (mesh, 0, 4, 5, 1, materialId);
	AddRectangle (mesh, 3, 2, 6, 7, materialId);
	
	return mesh;
}

Mesh GenerateCylinder (const Material& material, const glm::dmat4& transformation, double radius, double height, int segmentation, bool isSmooth)
{
	Mesh mesh;
	MaterialId materialId = mesh.AddMaterial (material);
	mesh.SetTransformation (transformation);

	double halfHeight = height / 2.0;
	double segmentAngle = (2.0 * PI) / double (segmentation);
	glm::dmat4 rotMatrix = glm::rotate (glm::dmat4 (1.0), segmentAngle, glm::dvec3 (0.0, 0.0, 1.0));
	
	glm::dvec3 bottomPoint (0.0, 0.0, -halfHeight);
	glm::dvec3 topPoint (0.0, 0.0, halfHeight);
	unsigned int bottomVertex = mesh.AddVertex (bottomPoint);
	unsigned int topVertex = mesh.AddVertex (topPoint);

	std::vector<unsigned int> bottomVertices = AddCircularVertices (mesh, bottomPoint, radius, segmentation);
	std::vector<unsigned int> topVertices = AddCircularVertices (mesh, topPoint, radius, segmentation);

	std::vector<unsigned int> bottomVerticesReversed = bottomVertices;
	std::reverse (bottomVerticesReversed.begin (), bottomVerticesReversed.end ());

	AddCenteredConvexPolygon (mesh, bottomVertex, bottomVerticesReversed, glm::dvec3 (0.0, 0.0, -1.0), materialId);
	AddCenteredConvexPolygon (mesh, topVertex, topVertices, glm::dvec3 (0.0, 0.0, 1.0), materialId);

	glm::dvec3 normal (1.0, 0.0, 0.0);
	for (size_t i = 0; i < segmentation; i++) {
		glm::dvec3 begNormal = normal;
		glm::dvec3 endNormal = rotMatrix * glm::dvec4 (normal, 1.0);
		unsigned int a = bottomVertices[i];
		unsigned int b = bottomVertices[i == segmentation - 1 ? 0 : i + 1];
		unsigned int c = topVertices[i == segmentation - 1 ? 0 : i + 1];
		unsigned int d = topVertices[i];
		if (isSmooth) {
			unsigned int n1 = mesh.AddNormal (begNormal);
			unsigned int n2 = mesh.AddNormal (endNormal);
			mesh.AddTriangle (a, b, c, n1, n2, n2, materialId);
			mesh.AddTriangle (a, c, d, n1, n2, n1, materialId);
		} else {
			AddRectangle (mesh, a, b, c, d, materialId);
		}
		normal = endNormal;
	}

	return mesh;
}

Mesh GenerateCone (const Material& material, const glm::dmat4& transformation, double topRadius, double bottomRadius, double height, int segmentation, bool isSmooth)
{
	Mesh mesh;
	MaterialId materialId = mesh.AddMaterial (material);
	mesh.SetTransformation (transformation);

	double halfHeight = height / 2.0;
	double segmentAngle = (2.0 * PI) / double (segmentation);
	glm::dmat4 rotMatrix = glm::rotate (glm::dmat4 (1.0), segmentAngle, glm::dvec3 (0.0, 0.0, 1.0));

	bool onePointBottom = Geometry::IsEqual (bottomRadius, 0.0);
	bool onePointTop = Geometry::IsEqual (topRadius, 0.0);

	std::vector<unsigned int> bottomVertices;
	if (onePointBottom) {
		bottomVertices.push_back (mesh.AddVertex (glm::dvec3 (0.0, 0.0, -halfHeight)));
	} else {
		glm::dvec3 bottomPoint (0.0, 0.0, -halfHeight);
		unsigned int bottomVertex = mesh.AddVertex (bottomPoint);
		bottomVertices = AddCircularVertices (mesh, bottomPoint, bottomRadius, segmentation);
		std::vector<unsigned int> bottomVerticesReversed = bottomVertices;
		std::reverse (bottomVerticesReversed.begin (), bottomVerticesReversed.end ());
		AddCenteredConvexPolygon (mesh, bottomVertex, bottomVerticesReversed, glm::dvec3 (0.0, 0.0, -1.0), materialId);
	}

	std::vector<unsigned int> topVertices;
	if (onePointTop) {
		topVertices.push_back (mesh.AddVertex (glm::dvec3 (0.0, 0.0, halfHeight)));
	} else {
		glm::dvec3 topPoint (0.0, 0.0, halfHeight);
		unsigned int topVertex = mesh.AddVertex (topPoint);
		topVertices = AddCircularVertices (mesh, topPoint, topRadius, segmentation);
		AddCenteredConvexPolygon (mesh, topVertex, topVertices, glm::dvec3 (0.0, 0.0, 1.0), materialId);
	}

	double coneAngle = atan (height / (bottomRadius - topRadius));
	double topCenterOffset = topRadius * tan (PI / 2.0 - coneAngle);
	double bottomCenterOffset = bottomRadius * tan (PI / 2.0 - coneAngle);
	glm::dvec3 topNormalCenter (0.0, 0.0, halfHeight - topCenterOffset);
	glm::dvec3 bottomNormalCenter (0.0, 0.0, -halfHeight - bottomCenterOffset);

	if (onePointTop) {
		for (size_t i = 0; i < segmentation; i++) {
			unsigned int a = bottomVertices[i];
			unsigned int b = bottomVertices[i == segmentation - 1 ? 0 : i + 1];
			unsigned int c = topVertices[0];
			if (isSmooth) {
				glm::dvec3 aNormal = glm::normalize (mesh.GetGeometry ().GetVertex (a) - bottomNormalCenter);
				glm::dvec3 bNormal = glm::normalize (mesh.GetGeometry ().GetVertex (b) - bottomNormalCenter);
				unsigned int an = mesh.AddNormal (aNormal);
				unsigned int bn = mesh.AddNormal (bNormal);
				unsigned int cn = mesh.AddNormal (glm::normalize ((aNormal + bNormal) / 2.0));
				mesh.AddTriangle (a, b, c, an, bn, cn, materialId);
			} else {
				mesh.AddTriangle (a, b, c, materialId);
			}
		}
	} else if (onePointBottom) {
		for (size_t i = 0; i < segmentation; i++) {
			unsigned int a = bottomVertices[0];
			unsigned int b = topVertices[i == segmentation - 1 ? 0 : i + 1];
			unsigned int c = topVertices[i];
			if (isSmooth) {
				glm::dvec3 bNormal = glm::normalize (mesh.GetGeometry ().GetVertex (b) - topNormalCenter);
				glm::dvec3 cNormal = glm::normalize (mesh.GetGeometry ().GetVertex (c) - topNormalCenter);
				unsigned int an = mesh.AddNormal (glm::normalize ((bNormal + cNormal) / 2.0));
				unsigned int bn = mesh.AddNormal (bNormal);
				unsigned int cn = mesh.AddNormal (cNormal);
				mesh.AddTriangle (a, b, c, an, bn, cn, materialId);
			} else {
				mesh.AddTriangle (a, b, c, materialId);
			}
		}
	} else {
		for (size_t i = 0; i < segmentation; i++) {
			unsigned int a = bottomVertices[i];
			unsigned int b = bottomVertices[i == segmentation - 1 ? 0 : i + 1];
			unsigned int c = topVertices[i == segmentation - 1 ? 0 : i + 1];
			unsigned int d = topVertices[i];
			if (isSmooth) {
				unsigned int an = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (a) - bottomNormalCenter));
				unsigned int bn = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (b) - bottomNormalCenter));
				unsigned int cn = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (c) - topNormalCenter));
				unsigned int dn = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (d) - topNormalCenter));
				mesh.AddTriangle (a, b, c, an, bn, cn, materialId);
				mesh.AddTriangle (a, c, d, an, cn, dn, materialId);
			} else {
				AddRectangle (mesh, a, b, c, d, materialId);
			}
		}
	}

	return mesh;
}

Mesh GenerateSphere (const Material& material, const glm::dmat4& transformation, double radius, int segmentation, bool isSmooth)
{
	Mesh mesh;
	MaterialId materialId = mesh.AddMaterial (material);
	mesh.SetTransformation (transformation);

	unsigned int segments = (unsigned int) segmentation;
	unsigned int circleSegments = segments * 2;

	unsigned int topVertex = mesh.AddVertex (SphericalToCartesian (radius, 0.0, 0.0));
	double segmentAngle = PI / double (segments);
	double theta = segmentAngle;
	for (unsigned int i = 0; i < segments - 1; i++) {
		double phi = 0.0;
		for (unsigned int j = 0; j < circleSegments; j++) {
			mesh.AddVertex (SphericalToCartesian (radius, theta, phi));
			phi += segmentAngle;
		}
		theta += segmentAngle;
	}
	unsigned int bottomVertex = mesh.AddVertex (SphericalToCartesian (radius, PI, 0.0));

	unsigned int topVertexNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (topVertex)));
	unsigned int bottomVertexNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (bottomVertex)));

	for (unsigned int i = 1; i <= segments; i++) {
		for (unsigned int j = 0; j < circleSegments; j++) {
			bool last = (j == circleSegments - 1);
			if (i == 1) {
				unsigned int offset = 1;
				unsigned int curr = offset + j;
				unsigned int next = last ? offset : curr + 1;
				if (isSmooth) {
					unsigned int currNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (curr)));
					unsigned int nextNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (next)));
					mesh.AddTriangle (curr, next, topVertex, currNormal, nextNormal, topVertexNormal, materialId);
				} else {
					mesh.AddTriangle (curr, next, topVertex, materialId);
				}
			} else if (i < segments) {
				unsigned int offset = (i - 1) * circleSegments + 1;
				unsigned int curr = offset + j;
				unsigned int next = last ? offset : curr + 1;
				unsigned int top = curr - circleSegments;
				unsigned int ntop = last ? offset - circleSegments : top + 1;
				if (isSmooth) {
					unsigned int currNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (curr)));
					unsigned int nextNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (next)));
					unsigned int topNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (top)));
					unsigned int ntopNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (ntop)));
					mesh.AddTriangle (curr, next, ntop, currNormal, nextNormal, ntopNormal, materialId);
					mesh.AddTriangle (curr, ntop, top, currNormal, ntopNormal, topNormal, materialId);
				} else {
					AddRectangle (mesh, curr, next, ntop, top, materialId);
				}
			} else if (i == segments) {
				unsigned int offset = (i - 2) * circleSegments + 1;
				unsigned int curr = offset + j;
				unsigned int next = last ? offset : curr + 1;
				if (isSmooth) {
					unsigned int currNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (curr)));
					unsigned int nextNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (next)));
					mesh.AddTriangle (curr, bottomVertex, next, currNormal, bottomVertexNormal, nextNormal, materialId);
				} else {
					mesh.AddTriangle (curr, bottomVertex, next, materialId);
				}
			}
		}
	}

	return mesh;
}

Mesh GenerateTorus (const Material& material, const glm::dmat4& transformation, double outerRadius, double innerRadius, int outerSegmentation, int innerSegmentation, bool isSmooth)
{
	Mesh mesh;
	MaterialId materialId = mesh.AddMaterial (material);
	mesh.SetTransformation (transformation);

	std::vector<glm::dvec3> innerCircle;
	glm::dmat4 xRotation = glm::rotate (glm::dmat4 (1.0), PI / 2.0, glm::dvec3 (1.0, 0.0, 0.0));
	glm::dvec3 firstOuterCenter (outerRadius, 0.0, 0.0);
	GenerateCircularPoints (firstOuterCenter, innerRadius, innerSegmentation, [&] (const glm::dvec3& point) {
		innerCircle.push_back (glm::dvec3 (xRotation * glm::dvec4 (point, 1.0)));
	});

	unsigned int outerSegments = (unsigned int) outerSegmentation;
	unsigned int innerSegments = (unsigned int) innerSegmentation;
	double segmentAngle = (2.0 * PI) / double (outerSegmentation);
	std::vector<glm::dvec3> outerCenterPoints;
	for (unsigned int i = 0; i < outerSegments; i++) {
		glm::dmat4 rotMatrix = glm::rotate (glm::dmat4 (1.0), i * segmentAngle, glm::dvec3 (0.0, 0.0, 1.0));
		outerCenterPoints.push_back (glm::dvec3 (rotMatrix * glm::dvec4 (firstOuterCenter, 1.0)));
		for (unsigned int j = 0; j < innerSegments; j++) {
			glm::dvec3 circlePoint = glm::dvec3 (rotMatrix * glm::dvec4 (innerCircle[j], 1.0));
			mesh.AddVertex (circlePoint);
		}
	}

	for (unsigned int i = 0; i < outerSegments; i++) {
		bool outerLast = (i == outerSegments - 1);
		for (unsigned j = 0; j < innerSegments; j++) {
			bool innerLast = (j == innerSegments - 1);
			unsigned int curr = i * innerSegments + j;
			unsigned int next = curr + innerSegments;
			unsigned int ntop = next + 1;
			unsigned int top = curr + 1;
			if (outerLast && innerLast) {
				next = j;
				ntop = 0;
				top = i * innerSegments;
			} else if (outerLast) {
				next = j;
				ntop = j + 1;
			} else if (innerLast) {
				ntop = (i + 1) * innerSegments;
				top = i * innerSegments;
			}
			if (isSmooth) {
				glm::dvec3 firstCenterPoint = outerCenterPoints[i];
				glm::dvec3 secondCenterPoint = outerCenterPoints[outerLast ? 0 : i + 1];
				unsigned int currNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (curr) - firstCenterPoint));
				unsigned int nextNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (next) - secondCenterPoint));
				unsigned int ntopNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (ntop) - secondCenterPoint));
				unsigned int topNormal = mesh.AddNormal (glm::normalize (mesh.GetGeometry ().GetVertex (top) - firstCenterPoint));
				mesh.AddTriangle (curr, next, ntop, currNormal, nextNormal, ntopNormal, materialId);
				mesh.AddTriangle (curr, ntop, top, currNormal, ntopNormal, topNormal, materialId);
			} else {
				mesh.AddTriangle (curr, next, ntop, materialId);
				mesh.AddTriangle (curr, ntop, top, materialId);
			}
		}
	}

	return mesh;
}

static class A {
public:
	A () {
		GenerateTorus (Material (glm::dvec3 (1.0, 0.0, 0.0)), glm::dmat4 (1.0), 10.0, 1.0, 10, 10, true);
	}
} a;

}