#ifndef TRANSFORMATIONNODES_HPP
#define TRANSFORMATIONNODES_HPP

#include "NE_GenericValue.hpp"
#include "BI_BasicUINode.hpp"
#include "Basic3DNodeValues.hpp"
#include "IncludeGLM.hpp"

class TransformationMatrixNode : public BI::BasicUINode
{
	SERIALIZABLE;

public:
	TransformationMatrixNode ();
	TransformationMatrixNode (const NE::String& name, const NUIE::Point& position);

	virtual void				Initialize () override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class TranslationMatrixNode : public TransformationMatrixNode
{
	DYNAMIC_SERIALIZABLE (TranslationMatrixNode);

public:
	TranslationMatrixNode ();
	TranslationMatrixNode (const NE::String& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class TranslationMatrixXYZNode : public TransformationMatrixNode
{
	DYNAMIC_SERIALIZABLE (TranslationMatrixXYZNode);

public:
	TranslationMatrixXYZNode ();
	TranslationMatrixXYZNode (const NE::String& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class RotationMatrixNode : public TransformationMatrixNode
{
	DYNAMIC_SERIALIZABLE (RotationMatrixNode);

public:
	RotationMatrixNode ();
	RotationMatrixNode (const NE::String& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class ScaleMatrixNode : public TransformationMatrixNode
{
	DYNAMIC_SERIALIZABLE (ScaleMatrixNode);

public:
	ScaleMatrixNode ();
	ScaleMatrixNode (const NE::String& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class MatrixCombinationNode : public TransformationMatrixNode
{
	DYNAMIC_SERIALIZABLE (MatrixCombinationNode);

public:
	MatrixCombinationNode ();
	MatrixCombinationNode (const NE::String& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
