#ifndef GEOMETRY_BOUNDINGSHAPES_HPP
#define GEOMETRY_BOUNDINGSHAPES_HPP

#include "IncludeGLM.hpp"

namespace Geometry
{

class BoundingBox
{
public:
	BoundingBox ();
	BoundingBox (const glm::dvec3& min, const glm::dvec3& max);

	bool				IsValid () const;
	void				AddPoint (const glm::dvec3& point);

	const glm::dvec3&	GetMin () const;
	const glm::dvec3&	GetMax () const;
	glm::dvec3			GetCenter () const;

private:
	bool		isValid;
	glm::dvec3	min;
	glm::dvec3	max;
};

}

#endif
