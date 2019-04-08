#include "Shape.hpp"

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

Modeler::ShapePtr Shape::Transform (const glm::dmat4& newTransformation) const
{
	Modeler::ShapePtr transformed = Clone ();
	transformed->SetTransformation (newTransformation * transformed->GetTransformation ());
	return transformed;
}

}
