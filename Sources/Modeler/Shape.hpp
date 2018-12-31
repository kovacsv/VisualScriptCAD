#ifndef MODELER_SHAPE_HPP
#define MODELER_SHAPE_HPP

#include "Mesh.hpp"

#include <memory>

namespace Modeler
{

class Shape;
typedef std::shared_ptr<Shape> ShapePtr;
typedef std::shared_ptr<const Shape> ShapeConstPtr;

class Shape
{
public:
	Shape (const glm::dmat4& transformation);
	virtual ~Shape ();

	const glm::dmat4&		GetTransformation () const;
	void					SetTransformation (const glm::dmat4& newTransformation);

	virtual bool			Check () const = 0;
	virtual ShapePtr		Clone () const = 0;
	virtual std::wstring	ToString () const = 0;
	virtual Mesh			GenerateMesh () const = 0;

protected:
	glm::dmat4	transformation;
};

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
