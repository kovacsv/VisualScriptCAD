#ifndef BOOST_EXPRESSIONCALCULATOR_HPP
#define BOOST_EXPRESSIONCALCULATOR_HPP

#include <string>
#include <unordered_map>

namespace BoostOperations
{

using IdentifierMap = std::unordered_map<std::wstring, double>;

double	EvaluateExpression (const std::wstring& exp, const IdentifierMap& identifierMap);
bool	EvaluateExpression (const std::wstring& exp, const IdentifierMap& identifierMap, double& result);

}

#endif
