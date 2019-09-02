#include "SimpleTest.hpp"
#include "Camera.hpp"

using namespace Modeler;

namespace CameraTest
{

TEST (IsValidCameraTest)
{
	ASSERT (IsValidCamera (glm::dvec3 (3.0, 2.0, 1.0), glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 1.0), 45.0, 0.1, 100.0));
	ASSERT (!IsValidCamera (glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 1.0), 45.0, 0.1, 100.0));
	ASSERT (!IsValidCamera (glm::dvec3 (3.0, 2.0, 1.0), glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 0.0), 45.0, 0.1, 100.0));
	ASSERT (!IsValidCamera (glm::dvec3 (0.0, 0.0, 1.0), glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 1.0), 45.0, 0.1, 100.0));
	ASSERT (!IsValidCamera (glm::dvec3 (0.0, 0.0, -1.0), glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 1.0), 45.0, 0.1, 100.0));
	ASSERT (!IsValidCamera (glm::dvec3 (3.0, 2.0, 1.0), glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 1.0), -45.0, 0.1, 100.0));
	ASSERT (!IsValidCamera (glm::dvec3 (3.0, 2.0, 1.0), glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 1.0), 0.0, 0.1, 100.0));
	ASSERT (!IsValidCamera (glm::dvec3 (3.0, 2.0, 1.0), glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 1.0), 45.0, -0.1, 100.0));
	ASSERT (!IsValidCamera (glm::dvec3 (3.0, 2.0, 1.0), glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 1.0), 45.0, 0.1, -100.0));
	ASSERT (!IsValidCamera (glm::dvec3 (3.0, 2.0, 1.0), glm::dvec3 (0.0, 0.0, 0.0), glm::dvec3 (0.0, 0.0, 1.0), 45.0, 100.0, 10.0));
}

}
