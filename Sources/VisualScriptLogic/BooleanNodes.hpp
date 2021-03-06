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
	enum class Operation
	{
		Difference,
		Intersection
	};

	BooleanNode ();
	BooleanNode (const NE::String& name, const NUIE::Point& position, Operation operation);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	Operation	operation;
};

class UnionNode : public ShapeNode
{
	DYNAMIC_SERIALIZABLE (UnionNode);

public:
	UnionNode ();
	UnionNode (const NE::String& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
