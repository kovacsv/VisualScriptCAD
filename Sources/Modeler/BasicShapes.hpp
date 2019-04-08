#ifndef MODELER_BASICSHAPES_HPP
#define MODELER_BASICSHAPES_HPP

#include "Shape.hpp"
#include "Mesh.hpp"
#include "MeshGenerators.hpp"

#include <memory>

namespace Modeler
{

class BoxShape : public Shape
{
public:
	BoxShape (const Material& material, const glm::dmat4& transformation, double xSize, double ySize, double zSize);
	virtual ~BoxShape ();

	virtual bool			Check () const override;
	virtual ShapePtr		Clone () const override;
	virtual std::wstring	ToString () const override;
	virtual Mesh			GenerateMesh () const override;

private:
	Material	material;
	double		xSize;
	double		ySize;
	double		zSize;
};

class CylinderShape : public Shape
{
public:
	CylinderShape (const Material& material, const glm::dmat4& transformation, double radius, double height, int segmentation, bool isSmooth);
	virtual ~CylinderShape ();

	virtual bool			Check () const override;
	virtual ShapePtr		Clone () const override;
	virtual std::wstring	ToString () const override;
	virtual Mesh			GenerateMesh () const override;

private:
	Material	material;
	double		radius;
	double		height;
	int			segmentation;
	bool		isSmooth;
};

class CylinderShellShape : public Shape
{
public:
	CylinderShellShape (const Material& material, const glm::dmat4& transformation, double radius, double height, double thickness, int segmentation, bool isSmooth);
	virtual ~CylinderShellShape ();

	virtual bool			Check () const override;
	virtual ShapePtr		Clone () const override;
	virtual std::wstring	ToString () const override;
	virtual Mesh			GenerateMesh () const override;

private:
	Material	material;
	double		radius;
	double		height;
	double		thickness;
	int			segmentation;
	bool		isSmooth;
};

class ConeShape : public Shape
{
public:
	ConeShape (const Material& material, const glm::dmat4& transformation, double topRadius, double bottomRadius, double height, int segmentation, bool isSmooth);
	virtual ~ConeShape ();

	virtual bool			Check () const override;
	virtual ShapePtr		Clone () const override;
	virtual std::wstring	ToString () const override;
	virtual Mesh			GenerateMesh () const override;

private:
	Material	material;
	double		topRadius;
	double		bottomRadius;
	double		height;
	int			segmentation;
	bool		isSmooth;
};

class SphereShape : public Shape
{
public:
	SphereShape (const Material& material, const glm::dmat4& transformation, double radius, int segmentation, bool isSmooth);
	virtual ~SphereShape ();

	virtual bool			Check () const override;
	virtual ShapePtr		Clone () const override;
	virtual std::wstring	ToString () const override;
	virtual Mesh			GenerateMesh () const override;

private:
	Material	material;
	double		radius;
	int			segmentation;
	bool		isSmooth;
};

class TorusShape : public Shape
{
public:
	TorusShape (const Material& material, const glm::dmat4& transformation, double outerRadius, double innerRadius, int outerSegmentation, int innerSegmentation, bool isSmooth);
	virtual ~TorusShape ();

	virtual bool			Check () const override;
	virtual ShapePtr		Clone () const override;
	virtual std::wstring	ToString () const override;
	virtual Mesh			GenerateMesh () const override;

private:
	Material	material;
	double		outerRadius;
	double		innerRadius;
	int			outerSegmentation;
	int			innerSegmentation;
	bool		isSmooth;
};

class PrismShape : public Shape
{
public:
	PrismShape (const Material& material, const glm::dmat4& transformation, const std::vector<glm::dvec2>& basePolygon, double height, const TriangulatorPtr& triangulator);
	virtual ~PrismShape ();

	virtual bool			Check () const override;
	virtual ShapePtr		Clone () const override;
	virtual std::wstring	ToString () const override;
	virtual Mesh			GenerateMesh () const override;

private:
	Material					material;
	std::vector<glm::dvec2>		basePolygon;
	double						height;
	TriangulatorPtr				triangulator;
};

class PlatonicShape : public Shape
{
public:
	PlatonicShape (const Material& material, const glm::dmat4& transformation, PlatonicSolidType type, double radius);
	virtual ~PlatonicShape ();

	virtual bool			Check () const override;
	virtual ShapePtr		Clone () const override;
	virtual std::wstring	ToString () const override;
	virtual Mesh			GenerateMesh () const override;

private:
	Material			material;
	PlatonicSolidType	type;
	double				radius;
};

class MeshShape : public Shape
{
public:
	MeshShape (const glm::dmat4& transformation, const Mesh& mesh);
	virtual ~MeshShape ();

	virtual bool			Check () const override;
	virtual ShapePtr		Clone () const override;
	virtual std::wstring	ToString () const override;
	virtual Mesh			GenerateMesh () const override;

private:
	Mesh	mesh;
};

}

#endif
