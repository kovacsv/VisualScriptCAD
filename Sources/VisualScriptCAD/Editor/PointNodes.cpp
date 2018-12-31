#include "PointNodes.hpp"
#include "NE_StringUtils.hpp"
#include "NE_SingleValues.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "Geometry.hpp"
#include "NodeValues.hpp"
#include "ReadWrite.hpp"

NE::SerializationInfo			PointNodeBase::serializationInfo (NE::ObjectVersion (1));
NE::DynamicSerializationInfo	PointNode::serializationInfo (NE::ObjectId ("{F387DF9E-CFAE-47D9-ABD5-F1195473A5C5}"), NE::ObjectVersion (1), PointNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	LinePointsNode::serializationInfo (NE::ObjectId ("{6228348D-9DE5-43DA-A3D8-7AA59BEE551A}"), NE::ObjectVersion (1), LinePointsNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	ArcPointsNode::serializationInfo (NE::ObjectId ("{968F0889-E537-4A0F-9D76-4E0378868AB2}"), NE::ObjectVersion (1), ArcPointsNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	TransformPointNode::serializationInfo (NE::ObjectId ("{8AC8484D-C80C-450B-AF86-B5E915179CED}"), NE::ObjectVersion (1), TransformPointNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	PointTranslationNode::serializationInfo (NE::ObjectId ("{23F99EC7-E0E1-42FB-88BD-239A27F1E24E}"), NE::ObjectVersion (1), PointTranslationNode::CreateSerializableInstance);

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

PointNode::PointNode () :
	PointNode (L"", NUIE::Point ())
{

}

PointNode::PointNode (const std::wstring& name, const NUIE::Point& position) :
	PointNodeBase (name, position)
{

}

void PointNode::Initialize ()
{
	PointNodeBase::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("x"), L"X", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("y"), L"Y", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("z"), L"Z", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("point"), L"Point")));
}

NE::ValuePtr PointNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr x = EvaluateSingleInputSlot (NE::SlotId ("x"), env);
	NE::ValuePtr y = EvaluateSingleInputSlot (NE::SlotId ("y"), env);
	NE::ValuePtr z = EvaluateSingleInputSlot (NE::SlotId ("z"), env);
	if (!NE::IsComplexType<NE::NumberValue> (x) || !NE::IsComplexType<NE::NumberValue> (y) || !NE::IsComplexType<NE::NumberValue> (z)) {
		return nullptr;
	}

	std::shared_ptr<BI::ValueCombinationFeature> valueCombination = BI::GetValueCombinationFeature (this);

	NE::ListValuePtr result (new NE::ListValue ());
	valueCombination->CombineValues ({x, y, z}, [&] (const NE::ValueCombination& combination) {
		glm::vec3 point (
			NE::NumberValue::ToFloat (combination.GetValue (0)),
			NE::NumberValue::ToFloat (combination.GetValue (1)),
			NE::NumberValue::ToFloat (combination.GetValue (2))
		);
		result->Push (NE::ValuePtr (new PointValue (point)));
		return true;
	});

	return result;
}

void PointNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	PointNodeBase::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<PointNode, NE::FloatValue> (parameterList, L"X", NUIE::ParameterType::Float, NE::SlotId ("x"));
	NUIE::RegisterSlotDefaultValueNodeParameter<PointNode, NE::FloatValue> (parameterList, L"Y", NUIE::ParameterType::Float, NE::SlotId ("y"));
	NUIE::RegisterSlotDefaultValueNodeParameter<PointNode, NE::FloatValue> (parameterList, L"Z", NUIE::ParameterType::Float, NE::SlotId ("z"));
}

NE::Stream::Status PointNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	PointNodeBase::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status PointNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	PointNodeBase::Write (outputStream);
	return outputStream.GetStatus ();
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

NE::ValuePtr LinePointsNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr begValue = EvaluateSingleInputSlot (NE::SlotId ("beg"), env);
	NE::ValuePtr endValue = EvaluateSingleInputSlot (NE::SlotId ("end"), env);
	NE::ValuePtr segmentationValue = EvaluateSingleInputSlot (NE::SlotId ("segmentation"), env);
	if (!NE::IsComplexType<PointValue> (begValue) || !NE::IsComplexType<PointValue> (endValue) || !NE::IsComplexType<NE::NumberValue> (segmentationValue)) {
		return nullptr;
	}

	std::shared_ptr<BI::ValueCombinationFeature> valueCombination = BI::GetValueCombinationFeature (this);

	NE::ListValuePtr result (new NE::ListValue ());
	bool success = valueCombination->CombineValues ({begValue, endValue, segmentationValue}, [&] (const NE::ValueCombination& combination) {
		glm::dvec3 beg (PointValue::Get (combination.GetValue (0)));
		glm::dvec3 end (PointValue::Get (combination.GetValue (1)));
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

NE::ValuePtr ArcPointsNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr radiusValue = EvaluateSingleInputSlot (NE::SlotId ("radius"), env);
	NE::ValuePtr angleValue = EvaluateSingleInputSlot (NE::SlotId ("angle"), env);
	NE::ValuePtr segmentationValue = EvaluateSingleInputSlot (NE::SlotId ("segmentation"), env);
	if (!NE::IsComplexType<NE::NumberValue> (radiusValue) || !NE::IsComplexType<NE::NumberValue> (angleValue) || !NE::IsComplexType<NE::NumberValue> (segmentationValue)) {
		return nullptr;
	}

	std::shared_ptr<BI::ValueCombinationFeature> valueCombination = BI::GetValueCombinationFeature (this);

	NE::ListValuePtr result (new NE::ListValue ());
	bool success = valueCombination->CombineValues ({radiusValue, angleValue, segmentationValue}, [&] (const NE::ValueCombination& combination) {
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
		glm::mat4 rotation = glm::rotate (glm::mat4 (1.0f), glm::radians (segmentAngle), glm::vec3 (0.0f, 0.0f, 1.f));
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

TransformPointNode::TransformPointNode () :
	TransformPointNode (L"", NUIE::Point ())
{

}

TransformPointNode::TransformPointNode (const std::wstring& name, const NUIE::Point& position) :
	PointNodeBase (name, position)
{

}

void TransformPointNode::Initialize ()
{
	PointNodeBase::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("point"), L"Point", NE::ValuePtr (new PointValue (glm::dvec3 (0.0, 0.0, 0.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("point"), L"Point")));
}

NE::ValuePtr TransformPointNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr pointValue = EvaluateSingleInputSlot (NE::SlotId ("point"), env);
	NE::ValuePtr transformationValue = EvaluateSingleInputSlot (NE::SlotId ("transformation"), env);
	if (!NE::IsComplexType<PointValue> (pointValue) || !NE::IsComplexType<TransformationValue> (transformationValue)) {
		return nullptr;
	}

	std::shared_ptr<BI::ValueCombinationFeature> valueCombination = BI::GetValueCombinationFeature (this);

	NE::ListValuePtr result (new NE::ListValue ());
	valueCombination->CombineValues ({pointValue, transformationValue}, [&] (const NE::ValueCombination& combination) {
		glm::dvec3 point (PointValue::Get (combination.GetValue (0)));
		glm::dmat4 transformation (TransformationValue::Get (combination.GetValue (1)));
		glm::dvec3 transformed = glm::dvec3 (transformation * glm::dvec4 (point, 1.0));
		result->Push (NE::ValuePtr (new PointValue (transformed)));
		return true;
	});

	return result;
}

NE::Stream::Status TransformPointNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	PointNodeBase::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TransformPointNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	PointNodeBase::Write (outputStream);
	return outputStream.GetStatus ();
}

PointTranslationNode::PointTranslationNode () :
	PointTranslationNode (L"", NUIE::Point ())
{

}

PointTranslationNode::PointTranslationNode (const std::wstring& name, const NUIE::Point& position) :
	BI::BasicUINode (name, position)
{

}

void PointTranslationNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("point"), L"Point", NE::ValuePtr (new PointValue (glm::dvec3 (0.0, 0.0, 0.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("transformation"), L"Transformation")));
}

NE::ValuePtr PointTranslationNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr pointValue = EvaluateSingleInputSlot (NE::SlotId ("point"), env);
	if (!NE::IsComplexType<PointValue> (pointValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	NE::FlatEnumerate (pointValue, [&] (const NE::ValuePtr& val) {
		glm::dvec3 offset (PointValue::Get (val));
		glm::dmat4 transformation = glm::translate (glm::dmat4 (1.0), offset);
		result->Push (NE::ValuePtr (new TransformationValue (transformation)));
	});

	return result;
}

NE::Stream::Status PointTranslationNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BI::BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status PointTranslationNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BI::BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}
