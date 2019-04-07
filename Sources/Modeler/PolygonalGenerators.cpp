#include "PolygonalGenerators.hpp"
#include "Geometry.hpp"

namespace Modeler
{

Triangulator::Triangulator ()
{

}

Triangulator::~Triangulator ()
{

}

NaiveTriangulator::NaiveTriangulator () :
	Triangulator ()
{
}

bool NaiveTriangulator::TriangulatePolygon (const std::vector<glm::dvec2>& points, std::vector<std::array<size_t, 3>>& result)
{
	if (points.size () < 3) {
		return false;
	}
	for (size_t i = 1; i < points.size () - 1; i++) {
		result.push_back ({ 0, i, i + 1 });
	}
	return true;
}

PrismGenerator::PrismGenerator (const Material& material, const glm::dmat4& transformation, double height) :
	material (material),
	transformation (transformation),
	height (height),
	basePolygon (),
	vertexCount (0)
{
}

PrismGenerator::~PrismGenerator ()
{
}

void PrismGenerator::AddVertex (const glm::dvec2& position, VertexType vertexType)
{
	basePolygon.push_back (position);
	basePolygonVertexTypes.push_back (vertexType);
	vertexCount += 1;
}

Mesh PrismGenerator::Generate () const
{
	if (vertexCount < 3) {
		return EmptyMesh;
	}

	Mesh mesh;
	MaterialId materialId = mesh.AddMaterial (material);
	mesh.SetTransformation (transformation);

	if (!AddTopAndBottomVertices (mesh)) {
		return EmptyMesh;
	}
	if (!AddTopAndBottomTriangles (mesh, materialId)) {
		return EmptyMesh;
	}
	if (!AddSideTriangles (mesh, materialId)) {
		return EmptyMesh;
	}

	return mesh;
}

bool PrismGenerator::AddTopAndBottomVertices (Mesh& mesh) const
{
	for (const glm::dvec2& point : basePolygon) {
		mesh.AddVertex (point.x, point.y, 0.0);
	}
	for (const glm::dvec2& point : basePolygon) {
		mesh.AddVertex (point.x, point.y, height);
	}
	return true;
}

bool PrismGenerator::AddSideTriangles (Mesh& mesh, MaterialId materialId) const
{
	std::vector<unsigned int> normals;
	for (unsigned int i = 0; i < vertexCount; i++) {
		normals.push_back (mesh.AddNormal (CalculateNormal (i)));
	}
	for (unsigned int i = 0; i < vertexCount; i++) {
		unsigned int curr = GetBottomVertex (i);
		unsigned int next = GetBottomVertex (i + 1);
		unsigned int ntop = GetTopVertex (i + 1);
		unsigned int top = GetTopVertex (i);
		VertexType vertexType = basePolygonVertexTypes[i];
		if (vertexType == VertexType::Sharp) {
			unsigned int normal = normals[i];
			mesh.AddTriangle (curr, next, ntop, normal, materialId);
			mesh.AddTriangle (curr, ntop, top, normal, materialId);
		} else if (vertexType == VertexType::Soft) {
			unsigned int currNormal = normals[i];
			unsigned int nextNormal = normals[i < vertexCount - 1 ? i + 1 : 0];
			mesh.AddTriangle (curr, next, ntop, currNormal, nextNormal, nextNormal, materialId);
			mesh.AddTriangle (curr, ntop, top, currNormal, nextNormal, currNormal, materialId);
		} else {
			throw std::logic_error ("invalid vertex type");
		}
	}
	return true;
}

glm::dvec3 PrismGenerator::CalculateNormal (unsigned int edgeIndex) const
{
	VertexType vertexType = basePolygonVertexTypes[edgeIndex];
	if (vertexType == VertexType::Sharp) {
		return CalculateSharpNormal (edgeIndex);
	} else if (vertexType == VertexType::Soft) {
		glm::dvec3 normal = CalculateSharpNormal (edgeIndex);
		glm::dvec3 prevNormal = CalculateSharpNormal (edgeIndex > 0 ? edgeIndex - 1 : vertexCount - 1);
		return glm::normalize ((prevNormal + normal) / 2.0);
	}
	throw std::logic_error ("invalid vertex type");
}

glm::dvec3 PrismGenerator::CalculateSharpNormal (unsigned int edgeIndex) const
{
	const glm::dvec2& beg = basePolygon[edgeIndex];
	const glm::dvec2& end = basePolygon[edgeIndex < vertexCount - 1 ? edgeIndex + 1 : 0];
	glm::dvec2 normal = glm::normalize (glm::dvec2 (
		end.y - beg.y,
		-end.x + beg.x
	));
	return glm::dvec3 (normal, 0.0);
}

unsigned int PrismGenerator::GetBottomVertex (unsigned int vertexIndex) const
{
	return vertexIndex < vertexCount ? vertexIndex : 0;
}

unsigned int PrismGenerator::GetTopVertex (unsigned int vertexIndex) const
{
	return vertexIndex < vertexCount ? vertexIndex + vertexCount : vertexCount;
}

TriangulatedPrismGenerator::TriangulatedPrismGenerator (const Material& material, const glm::dmat4& transformation, double height, Triangulator& triangulator) :
	PrismGenerator (material, transformation, height),
	triangulator (triangulator)
{
}

bool TriangulatedPrismGenerator::AddTopAndBottomTriangles (Mesh& mesh, MaterialId materialId) const
{
	std::vector<std::array<size_t, 3>> baseTriangles;
	if (!triangulator.TriangulatePolygon (basePolygon, baseTriangles)) {
		return false;
	}

	unsigned int bottomNormal = mesh.AddNormal (0.0, 0.0, -1.0);
	for (const std::array<size_t, 3>& triangle : baseTriangles) {
		mesh.AddTriangle (
			(unsigned int) triangle[0],
			(unsigned int) triangle[2],
			(unsigned int) triangle[1],
			bottomNormal,
			materialId
		);
	}

	unsigned int topNormal = mesh.AddNormal (0.0, 0.0, 1.0);
	for (const std::array<size_t, 3>& triangle : baseTriangles) {
		mesh.AddTriangle (
			vertexCount + (unsigned int) triangle[0],
			vertexCount + (unsigned int) triangle[1],
			vertexCount + (unsigned int) triangle[2],
			topNormal,
			materialId
		);
	}

	return true;
}

CenterPointTriangulatedPrismGenerator::CenterPointTriangulatedPrismGenerator (const Material& material, const glm::dmat4& transformation, const glm::dvec2& center, double height) :
	PrismGenerator (material, transformation, height),
	center (center)
{
}

bool CenterPointTriangulatedPrismGenerator::AddTopAndBottomTriangles (Mesh& mesh, MaterialId materialId) const
{
	unsigned int bottomVertex = mesh.AddVertex (glm::dvec3 (center, 0.0));
	unsigned int bottomNormal = mesh.AddNormal (0.0, 0.0, -1.0);
	for (unsigned int i = 0; i < vertexCount; i++) {
		mesh.AddTriangle (
			bottomVertex,
			GetBottomVertex (i + 1),
			GetBottomVertex (i),
			bottomNormal,
			materialId
		);
	}

	unsigned int topVertex = mesh.AddVertex (glm::dvec3 (center, height));
	unsigned int topNormal = mesh.AddNormal (0.0, 0.0, 1.0);
	for (unsigned int i = 0; i < vertexCount; i++) {
		mesh.AddTriangle (
			topVertex,
			GetTopVertex (i),
			GetTopVertex (i + 1),
			topNormal,
			materialId
		);
	}

	return true;
}

}
