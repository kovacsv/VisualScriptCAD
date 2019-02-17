#include "ExpressionCalculator.hpp"
#include "Geometry.hpp"

#include <list>

#include <boost/variant.hpp>
#include <boost/optional.hpp>

#pragma warning(push)
#pragma warning(disable:4100)
#pragma warning(disable:4127)
#pragma warning(disable:4459)
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#pragma warning(pop)

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct Nil {};
struct PrefixOperator;
struct BinaryOperator;
struct ExpOperator;
struct Number;
struct Identifier;
struct Parenthesized;
struct Expression;

typedef boost::variant<
	Nil,
	boost::recursive_wrapper<PrefixOperator>,
	boost::recursive_wrapper<ExpOperator>,
	boost::recursive_wrapper<Number>,
	boost::recursive_wrapper<Identifier>,
	boost::recursive_wrapper<Parenthesized>,
	boost::recursive_wrapper<Expression>
> Operand;

struct PrefixOperator
{
	std::wstring operatorString;
	Operand operand;
};

struct BinaryOperator
{
	std::wstring operatorString;
	Operand operand;
};

struct ExpOperator
{
	Operand left;
	boost::optional<Operand> right;
};

struct Number
{
	double value;
};

struct Identifier
{
	std::wstring name;
};

struct Parenthesized
{
	Operand operand;
};

struct Expression
{
	Operand head;
	std::vector<BinaryOperator> tail;
};

BOOST_FUSION_ADAPT_STRUCT( 
	PrefixOperator, 
	(std::wstring, operatorString)
	(Operand, operand) 
)

BOOST_FUSION_ADAPT_STRUCT( 
	BinaryOperator, 
	(std::wstring, operatorString)
	(Operand, operand) 
)

BOOST_FUSION_ADAPT_STRUCT( 
	ExpOperator, 
	(Operand, left)
	(boost::optional<Operand>, right) 
) 

BOOST_FUSION_ADAPT_STRUCT( 
	Identifier, 
	(std::wstring, name)
)

BOOST_FUSION_ADAPT_STRUCT( 
	Number, 
	(double, value)
)

BOOST_FUSION_ADAPT_STRUCT(
	Parenthesized,
	(Operand, operand)
)

BOOST_FUSION_ADAPT_STRUCT(
	Expression,
	(Operand, head)
	(std::vector<BinaryOperator>, tail)
)

template <typename Iterator>
struct BoostExpressionGrammar : qi::grammar<Iterator, Expression (), ascii::space_type>
{
	BoostExpressionGrammar () : BoostExpressionGrammar::base_type (expression)
	{
		qi::string_type string_;
		qi::real_parser<double, qi::real_policies<double>> double_;

		using qi::char_;
		using qi::alpha;
		using qi::alnum;
		using qi::as_wstring;

		expression = additiveExpression.alias ();

		additiveExpression =
			multiplicativeExpression >>
			*(
				(string_ (L"+") >> multiplicativeExpression) |
				(string_ (L"-") >> multiplicativeExpression)
			);

		multiplicativeExpression =
			exponentialExpression >>
			*(
				(string_ (L"*") >> exponentialExpression) |
				(string_ (L"/") >> exponentialExpression)
			);

		exponentialExpression = 
			prefixExpression
			>> -(L"^" >> exponentialExpression);

		prefixExpression =
			primaryExpression |
			(string_ (L"-") >> prefixExpression) |
			(string_ (L"+") >> prefixExpression);

		primaryExpression =
			numberExpression |
			identifierExpression |
			parenthesizedExpression;

		numberExpression =
			double_;

		identifierExpression =
			as_wstring[alpha >> *alnum];

		parenthesizedExpression =
			L"(" >> expression >> L")";
	}

	qi::rule<Iterator, Expression (), ascii::space_type> expression;
	qi::rule<Iterator, Expression (), ascii::space_type> additiveExpression;
	qi::rule<Iterator, Expression (), ascii::space_type> multiplicativeExpression;
	qi::rule<Iterator, ExpOperator (), ascii::space_type> exponentialExpression;
	qi::rule<Iterator, Operand (), ascii::space_type> prefixExpression;
	qi::rule<Iterator, Operand (), ascii::space_type> primaryExpression;
	qi::rule<Iterator, Identifier (), ascii::space_type> identifierExpression;
	qi::rule<Iterator, Number (), ascii::space_type> numberExpression;
	qi::rule<Iterator, Parenthesized (), ascii::space_type> parenthesizedExpression;
};

class BoostExpressionCalculator
{
public:
	typedef double result_type;

	BoostExpressionCalculator (const std::unordered_map<std::wstring, double>& identifierMap) :
		identifierMap (identifierMap)
	{

	}

	result_type operator() (const Nil& /*nil*/)
	{
		return 0.0;
	}

	result_type operator() (const Operand& operand)
	{
		return boost::apply_visitor (*this, operand);
	}

	result_type operator() (const Number& number)
	{
		return number.value;
	}

	result_type operator() (const Identifier& identifier)
	{
		auto found = identifierMap.find (identifier.name);
		if (found == identifierMap.end ()) {
			throw std::logic_error ("invalid identifier");
		}
		return found->second;
	}

	result_type operator() (const PrefixOperator& prefixOperator)
	{
		double multiplier = 1.0;
		if (prefixOperator.operatorString == L"+") {
			multiplier = 1.0;
		} else if (prefixOperator.operatorString == L"-") {
			multiplier = -1.0;
		} else {
			throw std::logic_error ("invalid prefix operator");
		}
		return multiplier * boost::apply_visitor (*this, prefixOperator.operand);
	}

	result_type operator() (const ExpOperator& expOperator)
	{
		double left = boost::apply_visitor (*this, expOperator.left);
		if (!expOperator.right) {
			return left;
		}

		double right = boost::apply_visitor (*this, expOperator.right.get ());
		return pow (left, right);
	}

	result_type operator() (const Parenthesized& parenthesized)
	{
		return boost::apply_visitor (*this, parenthesized.operand);
	}

	result_type operator() (const Expression& expression)
	{
		double result = boost::apply_visitor (*this, expression.head);
		for (size_t i = 0; i < expression.tail.size (); ++i) {
			const BinaryOperator& binaryOperator = expression.tail[i];
			double rightResult = boost::apply_visitor (*this, binaryOperator.operand);
			if (binaryOperator.operatorString == L"+") {
				result += rightResult;
			} else if (binaryOperator.operatorString == L"-") {
				result -= rightResult;
			} else if (binaryOperator.operatorString == L"*") {
				result *= rightResult;
			} else if (binaryOperator.operatorString == L"/") {
				if (Geometry::IsEqual (rightResult, 0.0)) {
					throw std::logic_error ("division by zero");
				}
				result /= rightResult;
			}
		}
		return result;
	}

	const std::unordered_map<std::wstring, double>& identifierMap;
};

namespace BoostOperations
{

double EvaluateExpression (const std::wstring& exp, const std::unordered_map<std::wstring, double>& identifierMap)
{
	std::wstring::const_iterator first = exp.begin ();
	std::wstring::const_iterator last = exp.end ();

	BoostExpressionGrammar<std::wstring::const_iterator> grammar;
	Expression resultExpression;
	bool parseSucceeded = qi::phrase_parse (first, last, grammar, qi::ascii::space, resultExpression);
	if (!parseSucceeded || first != last) {
		throw std::logic_error ("failed to parse expression");
	}

	BoostExpressionCalculator calculator (identifierMap);
	return calculator (resultExpression);
}

}
