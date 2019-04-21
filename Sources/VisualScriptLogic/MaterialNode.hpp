#ifndef MATERIALNODE_HPP
#define MATERIALNODE_HPP

#include "NE_GenericValue.hpp"
#include "BI_BasicUINode.hpp"
#include "Model.hpp"

class MaterialValue : public NE::GenericValue<Modeler::Material>
{
	DYNAMIC_SERIALIZABLE (MaterialValue);

public:
	MaterialValue ();
	MaterialValue (const Modeler::Material& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString (const NE::StringSettings& stringSettings) const override;
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class MaterialNode : public BI::BasicUINode
{
	DYNAMIC_SERIALIZABLE (MaterialNode);

public:
	MaterialNode ();
	MaterialNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
