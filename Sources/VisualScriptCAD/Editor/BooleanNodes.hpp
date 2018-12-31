#ifndef BOOLEANNODES_HPP
#define BOOLEANNODES_HPP

#include "Model.hpp"
#include "Shape.hpp"
#include "ShapeNode.hpp"
#include "BooleanOperations.hpp"

class BooleanNode : public ShapeNode
{
	DYNAMIC_SERIALIZABLE (BooleanNode);

public:
	BooleanNode ();
	BooleanNode (const std::wstring& name, const NUIE::Point& position, CGALOperations::BooleanOperation operation);

	virtual void				Initialize () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	CGALOperations::BooleanOperation operation;
};

#endif
