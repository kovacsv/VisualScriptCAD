#include "SimpleTest.hpp"
#include "Geometry.hpp"
#include "BoxUtils.hpp"

using namespace Geometry;

namespace BoxUtilsTest
{
	
TEST (BoxOverlapTest)
{
	ASSERT (AreBoxesOverlap (Box (glm::dvec3 (0, 0, 0), glm::dvec3 (1, 1, 1)), Box (glm::dvec3 (0, 0, 0), glm::dvec3 (1, 1, 1))));
	ASSERT (AreBoxesOverlap (Box (glm::dvec3 (0, 0, 0), glm::dvec3 (1, 1, 1)), Box (glm::dvec3 (0.5, 0.5, 0.5), glm::dvec3 (1, 1, 1))));
	ASSERT (AreBoxesOverlap (Box (glm::dvec3 (0, 0, 0), glm::dvec3 (1, 1, 1)), Box (glm::dvec3 (0.5, 0.5, 0.5), glm::dvec3 (1.5, 1.5, 1.5))));
	ASSERT (!AreBoxesOverlap (Box (glm::dvec3 (0, 0, 0), glm::dvec3 (1, 1, 1)), Box (glm::dvec3 (1, 1, 1), glm::dvec3 (1.5, 1.5, 1.5))));
	ASSERT (!AreBoxesOverlap (Box (glm::dvec3 (0, 0, 0), glm::dvec3 (1, 1, 1)), Box (glm::dvec3 (2, 2, 2), glm::dvec3 (3, 3, 3))));
}

}
