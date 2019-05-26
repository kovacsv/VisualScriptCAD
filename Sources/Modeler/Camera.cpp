#include "Camera.hpp"
#include "Geometry.hpp"

namespace Modeler
{

static const double MinZoomDistance = 1.0f;

Camera::Camera (const glm::dvec3& eye,
				const glm::dvec3& center,
				const glm::dvec3& up,
				double fieldOfView,
				double nearPlane,
				double farPlane) :
	eye (eye),
	center (center),
	up (up),
	fieldOfViewY (fieldOfView),
	nearPlane (nearPlane),
	farPlane (farPlane)
{
	
}

const glm::dvec3& Camera::GetEye () const
{
	return eye;
}

const glm::dvec3& Camera::GetCenter () const
{
	return center;
}

const glm::dvec3& Camera::GetUp () const
{
	return up;
}

glm::mat4 Camera::GetViewMatrix () const
{
	return glm::lookAt (eye, center, up);
}

glm::mat4 Camera::GetProjectionMatrix (int width, int height) const
{
	return glm::perspective (glm::radians (fieldOfViewY), (double) width / (double) height, nearPlane, farPlane);
}

static glm::dvec3 Rotate (const glm::dvec3& vec, const glm::dvec3& center, double angle, const glm::dvec3& normal)
{
	glm::dvec3 result = vec - center;
	result = glm::rotate (result, angle, normal);
	return glm::dvec3 (result + center);
}

void Camera::Orbit (double angleX, double angleY)
{
	double radAngleX = glm::radians (angleX);
	double radAngleY = glm::radians (angleY);
	
	glm::dvec3 viewDirection = glm::normalize (center - eye);
	glm::dvec3 upDirection = glm::normalize (up);
	glm::dvec3 horizontalDirection = glm::normalize (glm::cross (viewDirection, upDirection));

	// TODO: different center
	glm::dvec3 orbitCenter = center;

	double originalAngle = glm::angle (glm::normalize (eye - center), upDirection);
	double newAngle = originalAngle + radAngleY;
	if (Geometry::IsGreater (newAngle, 0.0) && Geometry::IsLower (newAngle, glm::pi<double> ())) {
		eye = Rotate (eye, orbitCenter, radAngleY, horizontalDirection);
		center = Rotate (center, orbitCenter, radAngleY, horizontalDirection);
	}

	eye = Rotate (eye, orbitCenter, radAngleX, upDirection);
	center = Rotate (center, orbitCenter, radAngleX, upDirection);
}

void Camera::Pan (double diffX, double diffY)
{
	glm::dvec3 viewDirection = glm::normalize (center - eye);
	glm::dvec3 upDirection = glm::normalize (up);
	glm::dvec3 horizontalDirection = glm::normalize (glm::cross (viewDirection, upDirection));
	glm::dvec3 verticalDirection = glm::normalize (glm::cross (horizontalDirection, viewDirection));

	double eyeCenterDistance = glm::length (center - eye);
	double relDiffX = diffX / 1000.0f * eyeCenterDistance;
	double relDiffY = diffY / 1000.0f * eyeCenterDistance;

	eye = eye + horizontalDirection * relDiffX + verticalDirection * relDiffY;
	center = center + horizontalDirection * relDiffX + verticalDirection * relDiffY;
}

void Camera::Zoom (double ratio)
{
	glm::dvec3 direction = center - eye;
	double distance = glm::length (direction);
	double movement = distance * ratio;
	bool zoomIn = ratio > 0.0f;
	if (zoomIn && distance < MinZoomDistance) {
		return;
	}
	eye = eye + glm::normalize (direction) * movement;
}

void Camera::ZoomToSphere (const glm::dvec3& sphereCenter, double sphereRadius, int width, int height)
{
	glm::dvec3 offetToOrigin = center - sphereCenter;
	center = sphereCenter;
	eye = eye - offetToOrigin;
	
	double calcFieldOfView = 0.0;
	double widthHeightRatio = (double) width / (double) height;
	if (widthHeightRatio >= 1.0f) {
		calcFieldOfView = fieldOfViewY / 2.0;
	} else {
		calcFieldOfView = fieldOfViewY / 2.0 * widthHeightRatio;
	}
	
	double distance = sphereRadius / glm::sin (glm::radians (calcFieldOfView));
	glm::dvec3 centerToEyeDirection = glm::normalize (eye - center);
	eye = center + centerToEyeDirection * distance;

	// TODO: different center
}

}
