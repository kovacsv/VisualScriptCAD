#include "SimpleTest.hpp"
#include "Geometry.hpp"
#include "ExpressionCalculator.hpp"

using namespace BoostOperations;

namespace BoostOperationsTest
{

static bool CheckExpression (const std::wstring& exp, double refValue, const IdentifierMap& identifierMap = {})
{
	double val = 0.0;
	if (!EvaluateExpression (exp, identifierMap, val)) {
		return false;
	}
	return Geometry::IsEqual (val, refValue);
}

static bool CheckInvalidExpression (const std::wstring& exp, const IdentifierMap& identifierMap = {})
{
	double dummy = 0.0;
	return !EvaluateExpression (exp, identifierMap, dummy);
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
	ASSERT (CheckExpression (L"4 ^ 3 ^ 2", 262144.0));
	ASSERT (CheckExpression (L" 2    +  3", 2.0 + 3.0));
}

TEST (ExpressionTest_InvalidExpressions)
{
	ASSERT (!ParseExpression (L"2 +"));
	ASSERT (CheckInvalidExpression (L"2 +"));

	ASSERT (!ParseExpression (L"2 2"));
	ASSERT (CheckInvalidExpression (L"2 2"));

	ASSERT (ParseExpression (L"a + b"));
	ASSERT (CheckInvalidExpression (L"a + b"));

	ASSERT (!ParseExpression (L"a 3"));
	ASSERT (CheckInvalidExpression (L"a 3"));

	ASSERT (ParseExpression (L"1.0 / 0.0"));
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

TEST (ExpressionTest_Identifiers)
{
	IdentifierMap identifierMap;
	identifierMap.insert ({ L"apple", 2.0 });
	identifierMap.insert ({ L"banana", 3.0 });

	ASSERT (CheckInvalidExpression (L"apple + orange", identifierMap));
	ASSERT (CheckExpression (L"apple + banana", 2.0 + 3.0, identifierMap));
	ASSERT (CheckExpression (L"-apple + 2", 0.0, identifierMap));
}

}
