#ifndef TRANSFORMSHAPENODES_HPP
#define TRANSFORMSHAPENODES_HPP

#include "ShapeNode.hpp"

class TransformShapeNode : public ShapeNode
{
	DYNAMIC_SERIALIZABLE (TransformShapeNode);

public:
	TransformShapeNode ();
	TransformShapeNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
