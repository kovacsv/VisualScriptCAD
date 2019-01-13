#ifndef SUBDIVISIONNODES_HPP
#define SUBDIVISIONNODES_HPP

#include "Model.hpp"
#include "Shape.hpp"
#include "ShapeNode.hpp"
#include "Subdivision.hpp"

class SubdivisionNode : public ShapeNode
{
	DYNAMIC_SERIALIZABLE (SubdivisionNode);

public:
	SubdivisionNode ();
	SubdivisionNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
