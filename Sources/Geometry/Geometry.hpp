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

bool IsEqual (double a, double b);
bool IsLower (double a, double b);
bool IsGreater (double a, double b);

}

#endif
