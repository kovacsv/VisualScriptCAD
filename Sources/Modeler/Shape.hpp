#ifndef MODELER_SHAPE_HPP
#define MODELER_SHAPE_HPP

#include "Mesh.hpp"
#include "MeshGenerators.hpp"

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
	Modeler::ShapePtr		Transform (const glm::dmat4& newTransformation) const;

	virtual bool			Check () const = 0;
	virtual ShapePtr		Clone () const = 0;
	virtual std::wstring	ToString () const = 0;
	virtual Mesh			GenerateMesh () const = 0;

protected:
	glm::dmat4	transformation;
};

}

#endif
