#ifndef TRANSFORMATIONNODES_HPP
#define TRANSFORMATIONNODES_HPP

#include "NE_GenericValue.hpp"
#include "BI_BasicUINode.hpp"
#include "NodeValues.hpp"
#include "IncludeGLM.hpp"

class TransformationNode : public BI::BasicUINode
{
	SERIALIZABLE;

public:
	TransformationNode ();
	TransformationNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class TranslationNode : public TransformationNode
{
	DYNAMIC_SERIALIZABLE (TranslationNode);

public:
	TranslationNode ();
	TranslationNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class RotationNode : public TransformationNode
{
	DYNAMIC_SERIALIZABLE (RotationNode);

public:
	RotationNode ();
	RotationNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class ScaleNode : public TransformationNode
{
	DYNAMIC_SERIALIZABLE (ScaleNode);

public:
	ScaleNode ();
	ScaleNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class CombinationNode : public TransformationNode
{
	DYNAMIC_SERIALIZABLE (CombinationNode);

public:
	CombinationNode ();
	CombinationNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class TransformPointNode : public TransformationNode
{
	DYNAMIC_SERIALIZABLE (TransformPointNode);

public:
	TransformPointNode ();
	TransformPointNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
