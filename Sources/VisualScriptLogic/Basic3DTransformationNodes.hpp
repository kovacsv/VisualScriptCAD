#ifndef BASIC3DTRANSFORMATIONNODES_HPP
#define BASIC3DTRANSFORMATIONNODES_HPP

#include "TransformationNodes.hpp"

class TransformPointNode : public TransformationMatrixNode
{
	DYNAMIC_SERIALIZABLE (TransformPointNode);

public:
	TransformPointNode ();
	TransformPointNode (const NE::String& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
