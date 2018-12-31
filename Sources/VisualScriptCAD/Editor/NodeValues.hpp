#ifndef NODEVALUES_HPP
#define NODEVALUES_HPP

#include "NE_GenericValue.hpp"
#include "IncludeGLM.hpp"

class PointValue : public NE::GenericValue<glm::vec3>
{
	DYNAMIC_SERIALIZABLE (PointValue);

public:
	PointValue ();
	PointValue (const glm::vec3& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString (const NE::StringSettings& stringSettings) const override;
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class TransformationValue : public NE::GenericValue<glm::mat4>
{
	DYNAMIC_SERIALIZABLE (TransformationValue);

public:
	TransformationValue ();
	TransformationValue (const glm::mat4& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString (const NE::StringSettings& stringSettings) const override;
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};


#endif
