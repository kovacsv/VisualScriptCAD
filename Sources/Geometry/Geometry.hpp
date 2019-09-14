#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

namespace Geometry
{

extern const float FINF;
extern const float FEPS;
	
extern const double INF;
extern const double EPS;

bool IsEqual (float a, float b);
bool IsLower (float a, float b);
bool IsGreater (float a, float b);
bool IsLowerOrEqual (float a, float b);
bool IsGreaterOrEqual (float a, float b);

bool IsEqual (double a, double b);
bool IsLower (double a, double b);
bool IsGreater (double a, double b);
bool IsLowerOrEqual (double a, double b);
bool IsGreaterOrEqual (double a, double b);

bool IsPositive (double a);
bool IsNegative (double a);
bool IsZero (double a);

}

#endif
