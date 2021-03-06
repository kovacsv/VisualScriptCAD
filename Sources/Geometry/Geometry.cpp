#include "Geometry.hpp"

#include <cmath>

namespace Geometry
{

const float FINF = 10.0e10f;
const float FEPS = 10e-5f;

const double INF = 10.0e10;
const double EPS = 10e-5;

bool IsEqual (float a, float b)
{
	return fabs (a - b) < FEPS;
}

bool IsLower (float a, float b)
{
	return b - a > FEPS;
}

bool IsGreater (float a, float b)
{
	return a - b > FEPS;
}

bool IsLowerOrEqual (float a, float b)
{
	return IsLower (a, b) || IsEqual (a, b);
}

bool IsGreaterOrEqual (float a, float b)
{
	return IsGreater (a, b) || IsEqual (a, b);
}

bool IsEqual (double a, double b)
{
	return fabs (a - b) < EPS;
}

bool IsLower (double a, double b)
{
	return b - a > EPS;
}

bool IsGreater (double a, double b)
{
	return a - b > EPS;
}

bool IsLowerOrEqual (double a, double b)
{
	return IsLower (a, b) || IsEqual (a, b);
}

bool IsGreaterOrEqual (double a, double b)
{
	return IsGreater (a, b) || IsEqual (a, b);
}

bool IsPositive (double a)
{
	return a > EPS;
}

bool IsNegative (double a)
{
	return a < EPS;
}

bool IsZero (double a)
{
	return fabs (a) < FEPS;
}

}
