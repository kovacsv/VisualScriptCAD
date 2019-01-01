#ifndef POINTNODES_HPP
#define POINTNODES_HPP

#include "NE_Stream.hpp"
#include "NE_StringSettings.hpp"
#include "NE_GenericValue.hpp"
#include "NE_Stream.hpp"
#include "NE_StringSettings.hpp"
#include "NE_GenericValue.hpp"
#include "BI_BasicUINode.hpp"
#include "IncludeGLM.hpp"

class PointNodeBase : public BI::BasicUINode
{
	SERIALIZABLE;

public:
	PointNodeBase ();
	PointNodeBase (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;
	virtual bool				IsForceCalculated () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class PointNode : public PointNodeBase
{
	DYNAMIC_SERIALIZABLE (PointNode);

public:
	PointNode ();
	PointNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class LinePointsNode : public PointNodeBase
{
	DYNAMIC_SERIALIZABLE (LinePointsNode);

public:
	LinePointsNode ();
	LinePointsNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class ArcPointsNode : public PointNodeBase
{
	DYNAMIC_SERIALIZABLE (ArcPointsNode);

public:
	ArcPointsNode ();
	ArcPointsNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class PointTranslationNode : public BI::BasicUINode
{
	DYNAMIC_SERIALIZABLE (PointTranslationNode);

public:
	PointTranslationNode ();
	PointTranslationNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
