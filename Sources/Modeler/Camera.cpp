#include "Camera.hpp"
#include "Geometry.hpp"

namespace Modeler
{

Camera::Camera (const glm::vec3& eye,
				const glm::vec3& center,
				const glm::vec3& up,
				float fieldOfView,
				float nearPlane,
				float farPlane) :
	eye (eye),
	center (center),
	up (up),
	fieldOfView (fieldOfView),
	nearPlane (nearPlane),
	farPlane (farPlane)
{
	
}

const glm::vec3& Camera::GetEye () const
{
	return eye;
}

const glm::vec3& Camera::GetCenter () const
{
	return center;
}

const glm::vec3& Camera::GetUp () const
{
	return up;
}

glm::mat4 Camera::GetViewMatrix () const
{
	return glm::lookAt (eye, center, up);
}

glm::mat4 Camera::GetProjectionMatrix (int width, int height) const
{
	return glm::perspective (glm::radians (fieldOfView), (float) width / (float) height, nearPlane, farPlane);
}

static glm::vec3 Rotate (const glm::vec3& vec, const glm::vec3& center, float angle, const glm::vec3& normal)
{
	glm::vec3 result = vec - center;
	result = glm::rotate (result, angle, normal);
	return glm::vec3 (result + center);
}

void Camera::Orbit (float angleX, float angleY)
{
	float radAngleX = glm::radians (angleX);
	float radAngleY = glm::radians (angleY);
	
	glm::vec3 viewDirection = glm::normalize (center - eye);
	glm::vec3 upDirection = glm::normalize (up);
	glm::vec3 horizontalDirection = glm::normalize (glm::cross (viewDirection, upDirection));

	// TODO: different center
	glm::vec3 orbitCenter = center;

	float originalAngle = glm::angle (glm::normalize (eye - center), upDirection);
	float newAngle = originalAngle + radAngleY;
	if (Geometry::IsGreater (newAngle, 0.0f) && Geometry::IsLower (newAngle, glm::pi<float> ())) {
		eye = Rotate (eye, orbitCenter, radAngleY, horizontalDirection);
		center = Rotate (center, orbitCenter, radAngleY, horizontalDirection);
	}

	eye = Rotate (eye, orbitCenter, radAngleX, upDirection);
	center = Rotate (center, orbitCenter, radAngleX, upDirection);
}

void Camera::Pan (float diffX, float diffY)
{
	glm::vec3 viewDirection = glm::normalize (center - eye);
	glm::vec3 upDirection = glm::normalize (up);
	glm::vec3 horizontalDirection = glm::normalize (glm::cross (viewDirection, upDirection));
	glm::vec3 verticalDirection = glm::normalize (glm::cross (horizontalDirection, viewDirection));

	float eyeCenterDistance = glm::length (center - eye);
	float relDiffX = diffX / 1000.0f * eyeCenterDistance;
	float relDiffY = diffY / 1000.0f * eyeCenterDistance;

	eye = eye + horizontalDirection * relDiffX + verticalDirection * relDiffY;
	center = center + horizontalDirection * relDiffX + verticalDirection * relDiffY;
}

void Camera::Zoom (float ratio)
{
	glm::vec3 direction = center - eye;
	float distance = glm::length (direction);
	float movement = distance * ratio;
	bool zoomIn = ratio > 0.0f;
	// TODO: set limit from outside
	if (zoomIn && distance < 1.0f) {
		return;
	}
	eye = eye + glm::normalize (direction) * movement;
}

void Camera::ZoomToSphere (const glm::vec3& sphereCenter, float sphereRadius, int width, int height)
{
	glm::vec3 offetToOrigin = center - sphereCenter;
	center = sphereCenter;
	eye = eye - offetToOrigin;
	
	float calcFieldOfView = 0.0;
	if (width >= height) {
		calcFieldOfView = fieldOfView / 2.0f;
	} else {
		calcFieldOfView = fieldOfView / 2.0f * width / height;
	}
	
	float distance = sphereRadius / glm::sin (glm::radians (calcFieldOfView));
	glm::vec3 centerToEyeDirection = glm::normalize (eye - center);
	eye = center + centerToEyeDirection * distance;

	// TODO: different center
}

}
