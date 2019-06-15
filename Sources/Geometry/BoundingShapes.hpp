#ifndef GEOMETRY_BOUNDINGSHAPES_HPP
#define GEOMETRY_BOUNDINGSHAPES_HPP

#include "IncludeGLM.hpp"
#include <functional>

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

	void				EnumerateBoundingPoints (const std::function<void (const glm::dvec3&)>& processor) const;
	BoundingBox			Transform (const glm::dmat4& transformation) const;

private:
	bool		isValid;
	glm::dvec3	min;
	glm::dvec3	max;
};

class BoundingSphere
{
public:
	BoundingSphere (const glm::dvec3& center);
	BoundingSphere (const glm::dvec3& center, double radius);

	bool				IsValid () const;
	void				AddPoint (const glm::dvec3& point);

	const glm::dvec3&	GetCenter () const;
	double				GetRadius () const;

private:
	bool		isValid;
	glm::dvec3	center;
	double		radius;
};

extern const BoundingBox InvalidBoundingBox;
extern const BoundingSphere InvalidBoundingSphere;

}

#endif
