#include "Shape.hpp"
#include "Geometry.hpp"
#include "MeshGenerators.hpp"

namespace Modeler
{

Shape::Shape (const glm::dmat4& transformation) :
	transformation (transformation)
{
}

Shape::~Shape ()
{
}

const glm::dmat4& Shape::GetTransformation () const
{
	return transformation;
}

void Shape::SetTransformation (const glm::dmat4& newTransformation)
{
	transformation = newTransformation;
}

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
	result.SetTransformation (transformation);
	return result;
}

}