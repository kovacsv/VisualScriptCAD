#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "IncludeGLM.hpp"

namespace Modeler
{

class Camera
{
public:
	Camera (const glm::vec3& eye,
			const glm::vec3& center,
			const glm::vec3& up,
			float fieldOfView,
			float nearPlane,
			float farPlane);

	const glm::vec3&	GetEye () const;
	const glm::vec3&	GetCenter () const;
	const glm::vec3&	GetUp () const;

	glm::mat4			GetViewMatrix () const;
	glm::mat4			GetProjectionMatrix (int width, int height) const;

	void				Orbit (float angleX, float angleY);
	void				Pan (float diffX, float diffY);
	void				Zoom (float ratio);
	void				ZoomToSphere (const glm::vec3& sphereCenter, float sphereRadius, int width, int height);

private:
	glm::vec3	eye;
	glm::vec3	center;
	glm::vec3	up;
	float		fieldOfView;
	float		nearPlane;
	float		farPlane;
};

}

#endif
