#include "SimpleTest.hpp"
#include "Geometry.hpp"
#include "ExpressionCalculator.hpp"

using namespace BoostOperations;

namespace BoostOperationsTest
{

static bool CheckExpression (const std::wstring& exp, double refValue)
{
	double val = EvaluateExpression (exp);
	return Geometry::IsEqual (val, refValue);
}

static bool CheckInvalidExpression (const std::wstring& exp)
{
	double wasThrow = false;
	try {
		EvaluateExpression (exp);
	} catch (...) {
		wasThrow = true;
	}
	return wasThrow;
}

TEST (ExpressionTest_DoubleParser)
{
	ASSERT (CheckExpression (L"0", 0.0));
	ASSERT (CheckExpression (L"0.0", 0.0));
	ASSERT (CheckExpression (L"1", 1.0));
	ASSERT (CheckExpression (L"1.0", 1.0));
	ASSERT (CheckExpression (L"1.5", 1.5));
	ASSERT (CheckExpression (L"-1.5", -1.5));
	ASSERT (CheckExpression (L"+1.5", 1.5));
}

TEST (ExpressionTest_BinaryOperators)
{
	ASSERT (CheckExpression (L"2 + 3", 2.0 + 3.0));
	ASSERT (CheckExpression (L"2 - 3", 2.0 - 3.0));
	ASSERT (CheckExpression (L"2 * 3", 2.0 * 3.0));
	ASSERT (CheckExpression (L"2 / 3", 2.0 / 3.0));
	ASSERT (CheckExpression (L"2 ^ 3", pow (2.0, 3.0)));
}

TEST (ExpressionTest_InvalidExpressions)
{
	ASSERT (CheckInvalidExpression (L"2 +"));
	ASSERT (CheckInvalidExpression (L"1.0 / 0.0"));
}

TEST (ExpressionTest_Parentheses)
{
	ASSERT (CheckExpression (L"(2)", 2.0));
	ASSERT (CheckExpression (L"((2))", 2.0));
	ASSERT (CheckExpression (L"2 + 3 * 6", 2.0 + 3.0 * 6.0));
	ASSERT (CheckExpression (L"(2 + 3) * 6", (2.0 + 3.0) * 6.0));
	ASSERT (CheckExpression (L"+(2 + 3) * 6", +(2.0 + 3.0) * 6.0));
	ASSERT (CheckExpression (L"-(2 + 3) * 6", -(2.0 + 3.0) * 6.0));
}

}
