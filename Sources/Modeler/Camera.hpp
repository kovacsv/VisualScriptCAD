#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "IncludeGLM.hpp"

namespace Modeler
{

class Camera
{
public:
	Camera (const glm::dvec3& eye,
			const glm::dvec3& center,
			const glm::dvec3& up,
			double fieldOfView,
			double nearPlane,
			double farPlane);

	const glm::dvec3&	GetEye () const;
	const glm::dvec3&	GetCenter () const;
	const glm::dvec3&	GetUp () const;

	glm::mat4			GetViewMatrix () const;
	glm::mat4			GetProjectionMatrix (int width, int height) const;

	void				Orbit (double angleX, double angleY);
	void				Pan (double diffX, double diffY);
	void				Zoom (double ratio);
	void				ZoomToSphere (const glm::dvec3& sphereCenter, double sphereRadius, int width, int height);

private:
	glm::dvec3	eye;
	glm::dvec3	center;
	glm::dvec3	up;
	double		fieldOfViewY;
	double		nearPlane;
	double		farPlane;
};

}

#endif
