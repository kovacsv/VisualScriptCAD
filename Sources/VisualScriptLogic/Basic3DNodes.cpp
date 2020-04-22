#include "Basic3DNodes.hpp"
#include "NE_SingleValues.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "Geometry.hpp"
#include "Basic3DNodeValues.hpp"

NE::SerializationInfo			PointNodeBase::serializationInfo (NE::ObjectVersion (1));
NE::SerializationInfo			CoordinateNode::serializationInfo (NE::ObjectVersion (1));
NE::DynamicSerializationInfo	Point2DNode::serializationInfo (NE::ObjectId ("{83495D36-2E33-4BEB-A152-F097CEC6D4DA}"), NE::ObjectVersion (1), Point2DNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	PointNode::serializationInfo (NE::ObjectId ("{F387DF9E-CFAE-47D9-ABD5-F1195473A5C5}"), NE::ObjectVersion (1), PointNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	VectorNode::serializationInfo (NE::ObjectId ("{72FD83E7-EE2A-4F16-BAEC-5447554B93A5}"), NE::ObjectVersion (1), VectorNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	LinePointsNode::serializationInfo (NE::ObjectId ("{6228348D-9DE5-43DA-A3D8-7AA59BEE551A}"), NE::ObjectVersion (1), LinePointsNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	ArcPointsNode::serializationInfo (NE::ObjectId ("{968F0889-E537-4A0F-9D76-4E0378868AB2}"), NE::ObjectVersion (1), ArcPointsNode::CreateSerializableInstance);

PointNodeBase::PointNodeBase () :
	PointNodeBase (L"", NUIE::Point ())
{

}

PointNodeBase::PointNodeBase (const std::wstring& name, const NUIE::Point& position) :
	BI::BasicUINode (name, position)
{

}

void PointNodeBase::Initialize ()
{
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
}

void PointNodeBase::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BI::BasicUINode::RegisterParameters (parameterList);
}

bool PointNodeBase::IsForceCalculated () const
{
	return true;
}

NE::Stream::Status PointNodeBase::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BI::BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status PointNodeBase::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BI::BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

Point2DNode::Point2DNode () :
	Point2DNode (L"", NUIE::Point ())
{

}

Point2DNode::Point2DNode (const std::wstring& name, const NUIE::Point& position) :
	PointNodeBase (name, position)
{

}

void Point2DNode::Initialize ()
{
	PointNodeBase::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("x"), L"X", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("y"), L"Y", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("point"), L"Point")));
}

NE::ValueConstPtr Point2DNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr x = EvaluateInputSlot (NE::SlotId ("x"), env);
	NE::ValueConstPtr y = EvaluateInputSlot (NE::SlotId ("y"), env);
	if (!NE::IsComplexType<NE::NumberValue> (x) || !NE::IsComplexType<NE::NumberValue> (y)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	BI::CombineValues (this, {x, y}, [&] (const NE::ValueCombination& combination) {
		glm::vec2 point (
			NE::NumberValue::ToFloat (combination.GetValue (0)),
			NE::NumberValue::ToFloat (combination.GetValue (1))
		);
		result->Push (NE::ValuePtr (new Point2DValue (point)));
		return true;
	});

	return result;
}

void Point2DNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	PointNodeBase::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<Point2DNode, NE::FloatValue> (parameterList, L"X", NUIE::ParameterType::Float, NE::SlotId ("x"));
	NUIE::RegisterSlotDefaultValueNodeParameter<Point2DNode, NE::FloatValue> (parameterList, L"Y", NUIE::ParameterType::Float, NE::SlotId ("y"));
}

NE::Stream::Status Point2DNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	PointNodeBase::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status Point2DNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	PointNodeBase::Write (outputStream);
	return outputStream.GetStatus ();
}

CoordinateNode::CoordinateNode () :
	CoordinateNode (L"", NUIE::Point ())
{

}

CoordinateNode::CoordinateNode (const std::wstring& name, const NUIE::Point& position) :
	PointNodeBase (name, position)
{

}

void CoordinateNode::Initialize ()
{
	PointNodeBase::Initialize ();
	glm::vec3 defaultValue = GetDefaultValue ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("x"), L"X", NE::ValuePtr (new NE::FloatValue (defaultValue.x)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("y"), L"Y", NE::ValuePtr (new NE::FloatValue (defaultValue.y)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("z"), L"Z", NE::ValuePtr (new NE::FloatValue (defaultValue.z)), NE::OutputSlotConnectionMode::Single)));
}

NE::ValueConstPtr CoordinateNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr x = EvaluateInputSlot (NE::SlotId ("x"), env);
	NE::ValueConstPtr y = EvaluateInputSlot (NE::SlotId ("y"), env);
	NE::ValueConstPtr z = EvaluateInputSlot (NE::SlotId ("z"), env);
	if (!NE::IsComplexType<NE::NumberValue> (x) || !NE::IsComplexType<NE::NumberValue> (y) || !NE::IsComplexType<NE::NumberValue> (z)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {x, y, z}, [&] (const NE::ValueCombination& combination) {
		glm::vec3 vec (
			NE::NumberValue::ToFloat (combination.GetValue (0)),
			NE::NumberValue::ToFloat (combination.GetValue (1)),
			NE::NumberValue::ToFloat (combination.GetValue (2))
		);
		NE::ValueConstPtr val = GetValueFromVector (vec);
		if (val == nullptr) {
			return false;
		}
		result->Push (val);
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return result;
}

void CoordinateNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	PointNodeBase::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<CoordinateNode, NE::FloatValue> (parameterList, L"X", NUIE::ParameterType::Float, NE::SlotId ("x"));
	NUIE::RegisterSlotDefaultValueNodeParameter<CoordinateNode, NE::FloatValue> (parameterList, L"Y", NUIE::ParameterType::Float, NE::SlotId ("y"));
	NUIE::RegisterSlotDefaultValueNodeParameter<CoordinateNode, NE::FloatValue> (parameterList, L"Z", NUIE::ParameterType::Float, NE::SlotId ("z"));
}

NE::Stream::Status CoordinateNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	PointNodeBase::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status CoordinateNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	PointNodeBase::Write (outputStream);
	return outputStream.GetStatus ();
}

PointNode::PointNode () :
	PointNode (L"", NUIE::Point ())
{

}

PointNode::PointNode (const std::wstring& name, const NUIE::Point& position) :
	CoordinateNode (name, position)
{

}

void PointNode::Initialize ()
{
	CoordinateNode::Initialize ();
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("point"), L"Point")));
}

NE::Stream::Status PointNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	CoordinateNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status PointNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	CoordinateNode::Write (outputStream);
	return outputStream.GetStatus ();
}

glm::vec3 PointNode::GetDefaultValue () const
{
	return glm::vec3 (0.0f, 0.0f, 0.0f);
}

NE::ValueConstPtr PointNode::GetValueFromVector (const glm::vec3 & vec) const
{
	return NE::ValuePtr (new PointValue (vec));
}

VectorNode::VectorNode () :
	VectorNode (L"", NUIE::Point ())
{

}

VectorNode::VectorNode (const std::wstring& name, const NUIE::Point& position) :
	CoordinateNode (name, position)
{

}

void VectorNode::Initialize ()
{
	CoordinateNode::Initialize ();
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("vector"), L"Vector")));
}

NE::Stream::Status VectorNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	CoordinateNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status VectorNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	CoordinateNode::Write (outputStream);
	return outputStream.GetStatus ();
}

glm::vec3 VectorNode::GetDefaultValue () const
{
	return glm::vec3 (0.0f, 0.0f, 1.0f);
}

NE::ValueConstPtr VectorNode::GetValueFromVector (const glm::vec3 & vec) const
{
	if (Geometry::IsEqual (glm::length (vec), 0.0f)) {
		return nullptr;
	}
	return NE::ValuePtr (new VectorValue (vec));
}

LinePointsNode::LinePointsNode () :
	LinePointsNode (L"", NUIE::Point ())
{

}

LinePointsNode::LinePointsNode (const std::wstring& name, const NUIE::Point& position) :
	PointNodeBase (name, position)
{

}

void LinePointsNode::Initialize ()
{
	PointNodeBase::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("beg"), L"Begin", NE::ValuePtr (new PointValue (glm::dvec3 (0.0, 0.0, 0.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("end"), L"End", NE::ValuePtr (new PointValue (glm::dvec3 (1.0, 0.0, 0.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("segmentation"), L"Segmentation", NE::ValuePtr (new NE::IntValue (5)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("points"), L"Points")));
}

NE::ValueConstPtr LinePointsNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr begValue = EvaluateInputSlot (NE::SlotId ("beg"), env);
	NE::ValueConstPtr endValue = EvaluateInputSlot (NE::SlotId ("end"), env);
	NE::ValueConstPtr segmentationValue = EvaluateInputSlot (NE::SlotId ("segmentation"), env);
	if (!NE::IsComplexType<CoordinateValue> (begValue) || !NE::IsComplexType<CoordinateValue> (endValue) || !NE::IsComplexType<NE::NumberValue> (segmentationValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	bool success = BI::CombineValues (this, {begValue, endValue, segmentationValue}, [&] (const NE::ValueCombination& combination) {
		glm::dvec3 beg (CoordinateValue::Get (combination.GetValue (0)));
		glm::dvec3 end (CoordinateValue::Get (combination.GetValue (1)));
		int segmentation = NE::NumberValue::ToInteger (combination.GetValue (2));
		if (segmentation < 2) {
			return false;
		}
		double segmentLength = glm::distance (beg, end) / (double) (segmentation - 1);
		if (Geometry::IsEqual (segmentLength, 0.0)) {
			return false;
		}
		glm::dvec3 normal = glm::normalize (end - beg);
		for (int i = 0; i < segmentation; i++) {
			glm::dvec3 point = beg + normal * (double) i * segmentLength;
			result->Push (NE::ValuePtr (new PointValue (point)));
		}
		return true;
	});

	if (!success) {
		return nullptr;
	}

	return result;
}

void LinePointsNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	PointNodeBase::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<LinePointsNode, NE::IntValue> (parameterList, L"Segmentation", NUIE::ParameterType::Integer, NE::SlotId ("segmentation"));
}

NE::Stream::Status LinePointsNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	PointNodeBase::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status LinePointsNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	PointNodeBase::Write (outputStream);
	return outputStream.GetStatus ();
}

ArcPointsNode::ArcPointsNode () :
	ArcPointsNode (L"", NUIE::Point ())
{

}

ArcPointsNode::ArcPointsNode (const std::wstring& name, const NUIE::Point& position) :
	PointNodeBase (name, position)
{

}

void ArcPointsNode::Initialize ()
{
	PointNodeBase::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("radius"), L"Radius", NE::ValuePtr (new NE::FloatValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("angle"), L"Angle", NE::ValuePtr (new NE::FloatValue (360.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("segmentation"), L"Segmentation", NE::ValuePtr (new NE::IntValue (5)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("points"), L"Points")));
}

NE::ValueConstPtr ArcPointsNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr radiusValue = EvaluateInputSlot (NE::SlotId ("radius"), env);
	NE::ValueConstPtr angleValue = EvaluateInputSlot (NE::SlotId ("angle"), env);
	NE::ValueConstPtr segmentationValue = EvaluateInputSlot (NE::SlotId ("segmentation"), env);
	if (!NE::IsComplexType<NE::NumberValue> (radiusValue) || !NE::IsComplexType<NE::NumberValue> (angleValue) || !NE::IsComplexType<NE::NumberValue> (segmentationValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	bool success = BI::CombineValues (this, {radiusValue, angleValue, segmentationValue}, [&] (const NE::ValueCombination& combination) {
		float radius = NE::NumberValue::ToFloat (combination.GetValue (0));
		float angle = NE::NumberValue::ToFloat (combination.GetValue (1));
		int segmentation = NE::NumberValue::ToInteger (combination.GetValue (2));
		if (!Geometry::IsGreater (radius, 0.0f) || !Geometry::IsGreater (angle, 0.0f) || Geometry::IsGreater (angle, 360.0f) || segmentation < 2) {
			return false;
		}
		float segmentAngle = angle / (float) (segmentation - 1);
		if (Geometry::IsEqual (angle, 360.0f)) {
			segmentAngle = angle / (float) segmentation;
		}
		glm::vec3 currentPoint (radius, 0.0f, 0.0f);
		glm::mat4 rotation = glm::rotate (glm::mat4 (1.0f), glm::radians (segmentAngle), glm::vec3 (0.0f, 0.0f, 1.0f));
		for (int i = 0; i < segmentation; i++) {
			result->Push (NE::ValuePtr (new PointValue (currentPoint)));
			currentPoint = glm::dvec3 (rotation * glm::dvec4 (currentPoint, 1.0));
		}
		return true;
	});

	if (!success) {
		return nullptr;
	}

	return result;
}

void ArcPointsNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	PointNodeBase::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<ArcPointsNode, NE::FloatValue> (parameterList, L"Radius", NUIE::ParameterType::Float, NE::SlotId ("radius"));
	NUIE::RegisterSlotDefaultValueNodeParameter<ArcPointsNode, NE::FloatValue> (parameterList, L"Angle", NUIE::ParameterType::Float, NE::SlotId ("angle"));
	NUIE::RegisterSlotDefaultValueNodeParameter<ArcPointsNode, NE::IntValue> (parameterList, L"Segmentation", NUIE::ParameterType::Integer, NE::SlotId ("segmentation"));
}

NE::Stream::Status ArcPointsNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	PointNodeBase::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ArcPointsNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	PointNodeBase::Write (outputStream);
	return outputStream.GetStatus ();
}
