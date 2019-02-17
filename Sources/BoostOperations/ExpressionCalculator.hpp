#ifndef BOOST_EXPRESSIONCALCULATOR_HPP
#define BOOST_EXPRESSIONCALCULATOR_HPP

#include <string>
#include <unordered_map>

namespace BoostOperations
{

double EvaluateExpression (const std::wstring& exp, const std::unordered_map<std::wstring, double>& identifierMap);

}

#endif
