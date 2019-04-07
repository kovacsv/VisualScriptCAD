#include "PolygonalGenerators.hpp"
#include "Geometry.hpp"
#include "TriangleUtils.hpp"

namespace Modeler
{

static void AddPolygonalVerticesToMesh (Mesh& mesh, const std::vector<glm::dvec2>& basePolygon, double z)
{
	for (const glm::dvec2& point : basePolygon) {
		mesh.AddVertex (point.x, point.y, z);
	}
}

class SidePolygonInterface
{
public:
	SidePolygonInterface ()
	{
	}

	virtual ~SidePolygonInterface ()
	{
	}

	virtual unsigned int					VertexCount () const = 0;
	virtual bool							IsReversedOrientation () const = 0;

	virtual glm::dvec3						CalculateNormal (unsigned int vertexIndex) const = 0;
	virtual unsigned int					GetBottomVertex (unsigned int vertexIndex) const = 0;
	virtual unsigned int					GetTopVertex (unsigned int vertexIndex) const = 0;
	virtual PolygonalGenerator::VertexType	GetVertexType (unsigned int vertexIndex) const = 0;
};

static void AddPolygonalSideTrianglesToMesh (Mesh& mesh, MaterialId materialId, SidePolygonInterface& sidePoly)
{
	unsigned int vertexCount = sidePoly.VertexCount ();
	bool isReversed = sidePoly.IsReversedOrientation ();

	std::vector<unsigned int> normals;
	for (unsigned int i = 0; i < vertexCount; i++) {
		glm::dvec3 normal = sidePoly.CalculateNormal (i);
		if (isReversed) {
			normal = normal * -1.0;
		}
		normals.push_back (mesh.AddNormal (normal));
	}

	for (unsigned int i = 0; i < vertexCount; i++) {
		unsigned int curr = sidePoly.GetBottomVertex (i);
		unsigned int next = sidePoly.GetBottomVertex (i + 1);
		unsigned int ntop = sidePoly.GetTopVertex (i + 1);
		unsigned int top = sidePoly.GetTopVertex (i);
		PolygonalGenerator::VertexType vertexType = sidePoly.GetVertexType (i);
		if (vertexType == PolygonalGenerator::VertexType::Sharp) {
			unsigned int normal = normals[i];
			if (isReversed) {
				mesh.AddTriangle (curr, ntop, next, normal, materialId);
				mesh.AddTriangle (curr, top, ntop, normal, materialId);
			} else {
				mesh.AddTriangle (curr, next, ntop, normal, materialId);
				mesh.AddTriangle (curr, ntop, top, normal, materialId);
			}
		} else if (vertexType == PolygonalGenerator::VertexType::Soft) {
			unsigned int currNormal = normals[i];
			unsigned int nextNormal = normals[i < vertexCount - 1 ? i + 1 : 0];
			if (isReversed) {
				mesh.AddTriangle (curr, ntop, next, currNormal, nextNormal, nextNormal, materialId);
				mesh.AddTriangle (curr, top, ntop, currNormal, currNormal, nextNormal, materialId);
			} else {
				mesh.AddTriangle (curr, next, ntop, currNormal, nextNormal, nextNormal, materialId);
				mesh.AddTriangle (curr, ntop, top, currNormal, nextNormal, currNormal, materialId);
			}
		} else {
			throw std::logic_error ("invalid vertex type");
		}
	}
}

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

PolygonalGenerator::PolygonalGenerator (const Material& material, const glm::dmat4& transformation, double height) :
	material (material),
	transformation (transformation),
	height (height),
	basePolygon (),
	vertexCount (0)
{
}

PolygonalGenerator::~PolygonalGenerator ()
{
}

void PolygonalGenerator::AddVertex (const glm::dvec2& position, VertexType vertexType)
{
	basePolygon.push_back (position);
	basePolygonVertexTypes.push_back (vertexType);
	vertexCount += 1;
}

Mesh PolygonalGenerator::Generate () const
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

glm::dvec3 PolygonalGenerator::CalculateNormal (unsigned int vertexIndex) const
{
	VertexType vertexType = basePolygonVertexTypes[vertexIndex];
	if (vertexType == VertexType::Sharp) {
		return CalculateSharpNormal (vertexIndex);
	} else if (vertexType == VertexType::Soft) {
		glm::dvec3 normal = CalculateSharpNormal (vertexIndex);
		glm::dvec3 prevNormal = CalculateSharpNormal (vertexIndex > 0 ? vertexIndex - 1 : vertexCount - 1);
		return glm::normalize ((prevNormal + normal) / 2.0);
	}
	throw std::logic_error ("invalid vertex type");
}

glm::dvec3 PolygonalGenerator::CalculateSharpNormal (unsigned int vertexIndex) const
{
	const glm::dvec2& beg = basePolygon[vertexIndex];
	const glm::dvec2& end = basePolygon[vertexIndex < vertexCount - 1 ? vertexIndex + 1 : 0];
	glm::dvec2 normal = glm::normalize (glm::dvec2 (
		end.y - beg.y,
		-end.x + beg.x
	));
	return glm::dvec3 (normal, 0.0);
}

PrismGenerator::PrismGenerator (const Material& material, const glm::dmat4& transformation, double height) :
	PolygonalGenerator (material, transformation, height)
{
}

PrismGenerator::~PrismGenerator ()
{
}

bool PrismGenerator::AddTopAndBottomVertices (Mesh& mesh) const
{
	AddPolygonalVerticesToMesh (mesh, basePolygon, 0.0);
	AddPolygonalVerticesToMesh (mesh, basePolygon, height);
	return true;
}

bool PrismGenerator::AddSideTriangles (Mesh& mesh, MaterialId materialId) const
{
	class PrismSidePolygonInterface : public SidePolygonInterface
	{
	public:
		PrismSidePolygonInterface (const PrismGenerator& generator) :
			generator (generator)
		{
		}

		virtual unsigned int VertexCount () const override
		{
			return generator.vertexCount;
		}

		virtual bool IsReversedOrientation () const override
		{
			return false;
		}

		virtual glm::dvec3 CalculateNormal (unsigned int vertexIndex) const override
		{
			return generator.CalculateNormal (vertexIndex);
		}

		virtual unsigned int GetBottomVertex (unsigned int vertexIndex) const override
		{
			return generator.GetBottomVertex (vertexIndex);
		}

		virtual unsigned int GetTopVertex (unsigned int vertexIndex) const override
		{
			return generator.GetTopVertex (vertexIndex);
		}

		virtual PolygonalGenerator::VertexType GetVertexType (unsigned int vertexIndex) const override
		{
			return generator.basePolygonVertexTypes[vertexIndex];
		}

	private:
		const PrismGenerator& generator;
	};

	PrismSidePolygonInterface sidePoly (*this);
	AddPolygonalSideTrianglesToMesh (mesh, materialId, sidePoly);
	return true;
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

PrismShellGenerator::PrismShellGenerator (const Material& material, const glm::dmat4& transformation, double height, double thickness) :
	PolygonalGenerator (material, transformation, height),
	thickness (thickness)
{
}

PrismShellGenerator::~PrismShellGenerator ()
{
}

bool PrismShellGenerator::AddTopAndBottomVertices (Mesh& mesh) const
{
	std::vector<glm::dvec2> innerPolygon;
	for (size_t i = 0; i < basePolygon.size (); i++) {
		const glm::dvec2& prev = basePolygon[i > 0 ? i - 1 : basePolygon.size () - 1];
		const glm::dvec2& curr = basePolygon[i];
		const glm::dvec2& next = basePolygon[i < basePolygon.size () - 1 ? i + 1 : 0];

		glm::dvec2 prevDir = glm::normalize (prev - curr);
		glm::dvec2 nextDir = glm::normalize (next - curr);

		double angle = 0.0;
		double bisectLength = 0.0;
		Geometry::Orientation orientation = Geometry::GetTriangleOrientation2D (prev, curr, next);
		if (orientation == Geometry::Orientation::CounterClockwise) {
			angle = glm::angle (prevDir, nextDir) / 2.0;
			bisectLength = thickness / sin (angle);
		} else if (orientation == Geometry::Orientation::Clockwise) {
			angle = PI - glm::angle (prevDir, nextDir) / 2.0;
			bisectLength = thickness / sin (angle);
		} else if (orientation == Geometry::Orientation::Invalid) {
			angle = PI / 2.0;
			bisectLength = thickness;
		}

		glm::dvec2 bisectorDir = glm::rotate (nextDir, angle);
		innerPolygon.push_back (curr + bisectorDir * bisectLength);
	}

	AddPolygonalVerticesToMesh (mesh, basePolygon, 0.0);
	AddPolygonalVerticesToMesh (mesh, basePolygon, height);
	AddPolygonalVerticesToMesh (mesh, innerPolygon, 0.0);
	AddPolygonalVerticesToMesh (mesh, innerPolygon, height);

	return true;
}

bool PrismShellGenerator::AddTopAndBottomTriangles (Mesh& mesh, MaterialId materialId) const
{
	unsigned int bottomNormal = mesh.AddNormal (0.0, 0.0, -1.0);
	for (unsigned int i = 0; i < vertexCount; i++) {
		unsigned int curr = GetBottomVertex (i);
		unsigned int next = GetBottomVertex (i + 1);
		unsigned int ntop = GetInnerBottomVertex (i + 1);
		unsigned int top = GetInnerBottomVertex (i);
		mesh.AddTriangle (curr, ntop, next, bottomNormal, materialId);
		mesh.AddTriangle (curr, top, ntop, bottomNormal, materialId);
	}

	unsigned int topNormal = mesh.AddNormal (0.0, 0.0, 1.0);
	for (unsigned int i = 0; i < vertexCount; i++) {
		unsigned int curr = GetTopVertex (i);
		unsigned int next = GetTopVertex (i + 1);
		unsigned int ntop = GetInnerTopVertex (i + 1);
		unsigned int top = GetInnerTopVertex (i);
		mesh.AddTriangle (curr, next, ntop, topNormal, materialId);
		mesh.AddTriangle (curr, ntop, top, topNormal, materialId);
	}

	return true;
}

bool PrismShellGenerator::AddSideTriangles (Mesh& mesh, MaterialId materialId) const
{
	class PrismShellSidePolygonInterface : public SidePolygonInterface
	{
	public:
		PrismShellSidePolygonInterface (const PrismShellGenerator& generator, bool outsidePolygon) :
			generator (generator),
			outsidePolygon (outsidePolygon)
		{
		}

		virtual unsigned int VertexCount () const override
		{
			return generator.vertexCount;
		}

		virtual bool IsReversedOrientation () const override
		{
			return !outsidePolygon;
		}

		virtual glm::dvec3 CalculateNormal (unsigned int vertexIndex) const override
		{
			return generator.CalculateNormal (vertexIndex);
		}

		virtual unsigned int GetBottomVertex (unsigned int vertexIndex) const override
		{
			return outsidePolygon ? generator.GetBottomVertex (vertexIndex) : generator.GetInnerBottomVertex (vertexIndex);
		}

		virtual unsigned int GetTopVertex (unsigned int vertexIndex) const override
		{
			return outsidePolygon ? generator.GetTopVertex (vertexIndex) : generator.GetInnerTopVertex (vertexIndex);
		}

		virtual PolygonalGenerator::VertexType GetVertexType (unsigned int vertexIndex) const override
		{
			return generator.basePolygonVertexTypes[vertexIndex];
		}

	private:
		const PrismShellGenerator& generator;
		bool outsidePolygon;
	};

	PrismShellSidePolygonInterface sidePoly (*this, true);
	AddPolygonalSideTrianglesToMesh (mesh, materialId, sidePoly);
	PrismShellSidePolygonInterface innerSidePoly (*this, false);
	AddPolygonalSideTrianglesToMesh (mesh, materialId, innerSidePoly);
	return true;
}

unsigned int PrismShellGenerator::GetBottomVertex (unsigned int vertexIndex) const
{
	return vertexIndex < vertexCount ? vertexIndex : 0;
}

unsigned int PrismShellGenerator::GetTopVertex (unsigned int vertexIndex) const
{
	return vertexIndex < vertexCount ? vertexIndex + vertexCount : vertexCount;
}

unsigned int PrismShellGenerator::GetInnerBottomVertex (unsigned int vertexIndex) const
{
	return 2 * vertexCount + GetBottomVertex (vertexIndex);
}

unsigned int PrismShellGenerator::GetInnerTopVertex (unsigned int vertexIndex) const
{
	return 2 * vertexCount + GetTopVertex (vertexIndex);
}

}
