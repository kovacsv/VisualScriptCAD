#include "BasicShapes.hpp"
#include "Geometry.hpp"
#include "TriangleUtils.hpp"

namespace Modeler
{

BoxShape::BoxShape (const Material& material, const glm::dmat4& transformation, double xSize, double ySize, double zSize) :
	Shape (transformation),
	material (material),
	xSize (xSize),
	ySize (ySize),
	zSize (zSize)
{
}

BoxShape::~BoxShape ()
{
}

bool BoxShape::Check () const
{
	return Geometry::IsGreater (xSize, 0.0) && Geometry::IsGreater (ySize, 0.0) && Geometry::IsGreater (zSize, 0.0);
}

ShapePtr BoxShape::Clone () const
{
	return ShapePtr (new BoxShape (*this));
}

std::wstring BoxShape::ToString () const
{
	return L"Box";
}

Mesh BoxShape::GenerateMesh () const
{
	return GenerateBox (material, transformation, xSize, ySize, zSize);
}

BoxShellShape::BoxShellShape (const Material& material, const glm::dmat4& transformation, double xSize, double ySize, double zSize, double thickness) :
	Shape (transformation),
	material (material),
	xSize (xSize),
	ySize (ySize),
	zSize (zSize),
	thickness (thickness)
{
}

BoxShellShape::~BoxShellShape ()
{
}

bool BoxShellShape::Check () const
{
	return Geometry::IsGreater (xSize, 0.0) && Geometry::IsGreater (ySize, 0.0) && Geometry::IsGreater (zSize, 0.0) && Geometry::IsGreater (thickness, 0.0) && Geometry::IsLower (thickness, std::min (xSize, ySize) / 2.0);
}

ShapePtr BoxShellShape::Clone () const
{
	return ShapePtr (new BoxShellShape (*this));
}

std::wstring BoxShellShape::ToString () const
{
	return L"Box Shell";
}

Mesh BoxShellShape::GenerateMesh () const
{
	return GenerateBoxShell (material, transformation, xSize, ySize, zSize, thickness);
}

CylinderShape::CylinderShape (const Material& material, const glm::dmat4& transformation, double radius, double height, int segmentation, bool isSmooth) :
	Shape (transformation),
	material (material),
	radius (radius),
	height (height),
	segmentation (segmentation),
	isSmooth (isSmooth)
{
}

CylinderShape::~CylinderShape ()
{
}

bool CylinderShape::Check () const
{
	return Geometry::IsGreater (radius, 0.0) && Geometry::IsGreater (height, 0.0) && segmentation >= 3;
}

ShapePtr CylinderShape::Clone () const
{
	return ShapePtr (new CylinderShape (*this));
}

std::wstring CylinderShape::ToString () const
{
	return L"Cylinder";
}

Mesh CylinderShape::GenerateMesh () const
{
	return GenerateCylinder (material, transformation, radius, height, segmentation, isSmooth);
}

CylinderShellShape::CylinderShellShape (const Material& material, const glm::dmat4& transformation, double radius, double height, int segmentation, bool isSmooth, double thickness) :
	Shape (transformation),
	material (material),
	radius (radius),
	height (height),
	segmentation (segmentation),
	isSmooth (isSmooth),
	thickness (thickness)
{
}

CylinderShellShape::~CylinderShellShape ()
{
}

bool CylinderShellShape::Check () const
{
	return Geometry::IsGreater (radius, 0.0) && Geometry::IsGreater (height, 0.0) && segmentation >= 3 && Geometry::IsGreater (thickness, 0.0) && Geometry::IsLower (thickness, radius);
}

ShapePtr CylinderShellShape::Clone () const
{
	return ShapePtr (new CylinderShellShape (*this));
}

std::wstring CylinderShellShape::ToString () const
{
	return L"Cylinder Shell";
}

Mesh CylinderShellShape::GenerateMesh () const
{
	return GenerateCylinderShell (material, transformation, radius, height, segmentation, isSmooth, thickness);
}

ConeShape::ConeShape (const Material& material, const glm::dmat4& transformation, double topRadius, double bottomRadius, double height, int segmentation, bool isSmooth) :
	Shape (transformation),
	material (material),
	topRadius (topRadius),
	bottomRadius (bottomRadius),
	height (height),
	segmentation (segmentation),
	isSmooth (isSmooth)
{
}

ConeShape::~ConeShape ()
{
}

bool ConeShape::Check () const
{
	if (Geometry::IsLower (topRadius, 0.0) || Geometry::IsLower (bottomRadius, 0.0)) {
		return false;
	}
	if (Geometry::IsEqual (topRadius, 0.0) && Geometry::IsEqual (bottomRadius, 0.0)) {
		return false;
	}
	return Geometry::IsGreater (height, 0.0) && segmentation >= 3;
}

ShapePtr ConeShape::Clone () const
{
	return ShapePtr (new ConeShape (*this));
}

std::wstring ConeShape::ToString () const
{
	return L"Cone";
}

Mesh ConeShape::GenerateMesh () const
{
	return GenerateCone (material, transformation, topRadius, bottomRadius, height, segmentation, isSmooth);
}

SphereShape::SphereShape (const Material& material, const glm::dmat4& transformation, double radius, int segmentation, bool isSmooth) :
	Shape (transformation),
	material (material),
	radius (radius),
	segmentation (segmentation),
	isSmooth (isSmooth)
{
}

SphereShape::~SphereShape ()
{
}

bool SphereShape::Check () const
{
	return Geometry::IsGreater (radius, 0.0) && segmentation >= 3;
}

ShapePtr SphereShape::Clone () const
{
	return ShapePtr (new SphereShape (*this));
}

std::wstring SphereShape::ToString () const
{
	return L"Sphere";
}

Mesh SphereShape::GenerateMesh () const
{
	return GenerateSphere (material, transformation, radius, segmentation, isSmooth);
}

TorusShape::TorusShape (const Material& material, const glm::dmat4& transformation, double outerRadius, double innerRadius, int outerSegmentation, int innerSegmentation, bool isSmooth) :
	Shape (transformation),
	material (material),
	outerRadius (outerRadius),
	innerRadius (innerRadius),
	outerSegmentation (outerSegmentation),
	innerSegmentation (innerSegmentation),
	isSmooth (isSmooth)
{
}

TorusShape::~TorusShape ()
{
}

bool TorusShape::Check () const
{
	return Geometry::IsGreater (outerRadius, 0.0) && Geometry::IsGreater (innerRadius, 0.0)  && outerSegmentation >= 3 && innerSegmentation >= 3;
}

ShapePtr TorusShape::Clone () const
{
	return ShapePtr (new TorusShape (*this));
}

std::wstring TorusShape::ToString () const
{
	return L"Torus";
}

Mesh TorusShape::GenerateMesh () const
{
	return GenerateTorus (material, transformation, outerRadius, innerRadius, outerSegmentation, innerSegmentation, isSmooth);
}

PrismShape::PrismShape (const Material& material, const glm::dmat4& transformation, const std::vector<glm::dvec2>& basePolygon, double height, const TriangulatorPtr& triangulator) :
	Shape (transformation),
	material (material),
	basePolygon (basePolygon),
	height (height),
	triangulator (triangulator)
{
}

PrismShape::~PrismShape ()
{
}

bool PrismShape::Check () const
{
	if (basePolygon.size () < 3 || !Geometry::IsGreater (height, 0.0)) {
		return false;
	}
	if (Geometry::GetPolygonOrientation2D (basePolygon) != Geometry::Orientation::CounterClockwise) {
		return false;
	}
	std::vector<std::array<size_t, 3>> triangles;
	if (triangulator == nullptr || !triangulator->TriangulatePolygon (basePolygon, triangles)) {
		return false;
	}
	return true;
}

ShapePtr PrismShape::Clone () const
{
	return ShapePtr (new PrismShape (*this));
}

std::wstring PrismShape::ToString () const
{
	return L"Prism";
}

Mesh PrismShape::GenerateMesh () const
{
	return GeneratePrism (material, transformation, basePolygon, height, *triangulator);
}

PrismShellShape::PrismShellShape (const Material& material, const glm::dmat4& transformation, const std::vector<glm::dvec2>& basePolygon, double height, double thickness) :
	Shape (transformation),
	material (material),
	basePolygon (basePolygon),
	height (height),
	thickness (thickness)
{
}

PrismShellShape::~PrismShellShape ()
{
}

bool PrismShellShape::Check () const
{
	if (basePolygon.size () < 3 || !Geometry::IsGreater (height, 0.0) || !Geometry::IsGreater (thickness, 0.0)) {
		return false;
	}
	if (Geometry::GetPolygonOrientation2D (basePolygon) != Geometry::Orientation::CounterClockwise) {
		return false;
	}
	return true;
}

ShapePtr PrismShellShape::Clone () const
{
	return ShapePtr (new PrismShellShape (*this));
}

std::wstring PrismShellShape::ToString () const
{
	return L"Prism Shell";
}

Mesh PrismShellShape::GenerateMesh () const
{
	return GeneratePrismShell (material, transformation, basePolygon, height, thickness);
}

PlatonicShape::PlatonicShape (const Material& material, const glm::dmat4& transformation, PlatonicSolidType type, double radius) :
	Shape (transformation),
	material (material),
	type (type),
	radius (radius)
{
}

PlatonicShape::~PlatonicShape ()
{
}

bool PlatonicShape::Check () const
{
	if (!Geometry::IsGreater (radius, 0.0)) {
		return false;
	}
	return true;
}

ShapePtr PlatonicShape::Clone () const
{
	return ShapePtr (new PlatonicShape (*this));
}

std::wstring PlatonicShape::ToString () const
{
	switch (type) {
		case PlatonicSolidType::Tetrahedron:
			return L"Tetrahedron";
		case PlatonicSolidType::Octahedron:
			return L"Octahedron";
		case PlatonicSolidType::Hexahedron:
			return L"Hexahedron";
		case PlatonicSolidType::Dodecahedron:
			return L"Dodecahedron";
		case PlatonicSolidType::Icosahedron:
			return L"Icosahedron";
	}
	return L"Platonic";
}

Mesh PlatonicShape::GenerateMesh () const
{
	return GeneratePlatonicSolid (material, transformation, type, radius);
}

MeshShape::MeshShape (const glm::dmat4& transformation, const Mesh& mesh) :
	Shape (transformation),
	mesh (mesh)
{
}

MeshShape::~MeshShape ()
{
}

bool MeshShape::Check () const
{
	return true;
}

ShapePtr MeshShape::Clone () const
{
	return ShapePtr (new MeshShape (*this));
}

std::wstring MeshShape::ToString () const
{
	return L"Mesh";
}

Mesh MeshShape::GenerateMesh () const
{
	Mesh result = mesh;
	result.AddTransformation (transformation);
	return result;
}

}
