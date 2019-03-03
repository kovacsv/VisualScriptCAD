#ifndef PRISMNODE_HPP
#define PRISMNODE_HPP

#include "ShapeNode.hpp"

class PrismNode : public ShapeNode
{
	DYNAMIC_SERIALIZABLE (PrismNode);

public:
	PrismNode ();
	PrismNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
