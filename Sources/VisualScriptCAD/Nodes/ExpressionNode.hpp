#ifndef EXPRESSIONNODE_HPP
#define EXPRESSIONNODE_HPP

#include "BI_BasicUINode.hpp"

class ExpressionNode : public BI::BasicUINode
{
	DYNAMIC_SERIALIZABLE (ExpressionNode);

public:
	ExpressionNode ();
	ExpressionNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	const std::wstring&			GetExpression () const;
	void						SetExpression (const std::wstring& newExpression);

private:
	std::wstring expression;
};

#endif
