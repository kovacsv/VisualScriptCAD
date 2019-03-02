#ifndef BASIC3DNODES_HPP
#include "NE_Stream.hpp"
#include "NE_StringSettings.hpp"
#include "NE_GenericValue.hpp"
#include "NE_Stream.hpp"
#include "NE_StringSettings.hpp"
#include "NE_GenericValue.hpp"
#include "BI_BasicUINode.hpp"
#include "Basic3DNodeValues.hpp"

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

class Point2DNode : public PointNodeBase
{
	DYNAMIC_SERIALIZABLE (Point2DNode);

public:
	Point2DNode ();
	Point2DNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class CoordinateNode : public PointNodeBase
{
	SERIALIZABLE;

public:
	CoordinateNode ();
	CoordinateNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	virtual glm::vec3			GetDefaultValue () const = 0;
	virtual NE::ValueConstPtr	GetValueFromVector (const glm::vec3& vec) const = 0;
};

class PointNode : public CoordinateNode
{
	DYNAMIC_SERIALIZABLE (PointNode);

public:
	PointNode ();
	PointNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual glm::vec3			GetDefaultValue () const override;
	virtual NE::ValueConstPtr	GetValueFromVector (const glm::vec3& vec) const override;
};

class VectorNode : public CoordinateNode
{
	DYNAMIC_SERIALIZABLE (VectorNode);

public:
	VectorNode ();
	VectorNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual glm::vec3			GetDefaultValue () const override;
	virtual NE::ValueConstPtr	GetValueFromVector (const glm::vec3& vec) const override;
};

class LinePointsNode : public PointNodeBase
{
	DYNAMIC_SERIALIZABLE (LinePointsNode);

public:
	LinePointsNode ();
	LinePointsNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
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
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
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
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
