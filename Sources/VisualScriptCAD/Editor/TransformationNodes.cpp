#include "TransformationNodes.hpp"
#include "NE_SingleValues.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "ReadWrite.hpp"

NE::SerializationInfo			TransformationNode::serializationInfo (NE::ObjectVersion (1));
NE::DynamicSerializationInfo	TranslationNode::serializationInfo (NE::ObjectId ("{7F1B6F2F-2DE7-48FC-A926-5137FB9CFFF5}"), NE::ObjectVersion (1), TranslationNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	RotationNode::serializationInfo (NE::ObjectId ("{2F11941E-6B80-49A0-A553-BD230AC0336C}"), NE::ObjectVersion (1), RotationNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	ScaleNode::serializationInfo (NE::ObjectId ("{9E070858-8AC9-42DE-95A6-0C2AB6807426}"), NE::ObjectVersion (1), ScaleNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	CombinationNode::serializationInfo (NE::ObjectId ("{E6FC2758-00D6-47F6-B321-AE46ECEA66C5}"), NE::ObjectVersion (1), CombinationNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	TransformPointNode::serializationInfo (NE::ObjectId ("{8AC8484D-C80C-450B-AF86-B5E915179CED}"), NE::ObjectVersion (1), TransformPointNode::CreateSerializableInstance);

TransformationNode::TransformationNode () :
	TransformationNode (L"", NUIE::Point ())
{

}

TransformationNode::TransformationNode (const std::wstring& name, const NUIE::Point& position) :
	BI::BasicUINode (name, position)
{

}

void TransformationNode::Initialize ()
{
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
}

NE::Stream::Status TransformationNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BI::BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TransformationNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BI::BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

TranslationNode::TranslationNode () :
	TranslationNode (L"", NUIE::Point ())
{

}

TranslationNode::TranslationNode (const std::wstring& name, const NUIE::Point& position) :
	TransformationNode (name, position)
{

}

void TranslationNode::Initialize ()
{
	TransformationNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("offsetx"), L"Offset X", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("offsety"), L"Offset Y", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("offsetz"), L"Offset Z", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("transformation"), L"Transformation")));
}

NE::ValuePtr TranslationNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr offsetXValue = EvaluateSingleInputSlot (NE::SlotId ("offsetx"), env);
	NE::ValuePtr offsetYValue = EvaluateSingleInputSlot (NE::SlotId ("offsety"), env);
	NE::ValuePtr offsetZValue = EvaluateSingleInputSlot (NE::SlotId ("offsetz"), env);

	if (!NE::IsComplexType<NE::NumberValue> (offsetXValue) || !NE::IsComplexType<NE::NumberValue> (offsetYValue) || !NE::IsComplexType<NE::NumberValue> (offsetZValue)) {
		return nullptr;
	}

	std::shared_ptr<BI::ValueCombinationFeature> valueCombination = BI::GetValueCombinationFeature (this);

	NE::ListValuePtr result (new NE::ListValue ());
	valueCombination->CombineValues ({offsetXValue, offsetYValue, offsetZValue}, [&] (const NE::ValueCombination& combination) {
		glm::vec3 offset (
			NE::NumberValue::ToFloat (combination.GetValue (0)),
			NE::NumberValue::ToFloat (combination.GetValue (1)),
			NE::NumberValue::ToFloat (combination.GetValue (2))
		);
		glm::mat4 transformation = glm::translate (glm::mat4 (1.0f), offset);
		result->Push (NE::ValuePtr (new TransformationValue (transformation)));
		return true;
	});

	return result;
}

void TranslationNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	TransformationNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<TranslationNode, NE::FloatValue> (parameterList, L"Offset X", NUIE::ParameterType::Float, NE::SlotId ("offsetx"));
	NUIE::RegisterSlotDefaultValueNodeParameter<TranslationNode, NE::FloatValue> (parameterList, L"Offset Y", NUIE::ParameterType::Float, NE::SlotId ("offsety"));
	NUIE::RegisterSlotDefaultValueNodeParameter<TranslationNode, NE::FloatValue> (parameterList, L"Offset Z", NUIE::ParameterType::Float, NE::SlotId ("offsetz"));
}

NE::Stream::Status TranslationNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	TransformationNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TranslationNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	TransformationNode::Write (outputStream);
	return outputStream.GetStatus ();
}

RotationNode::RotationNode () :
	RotationNode (L"", NUIE::Point ())
{

}

RotationNode::RotationNode (const std::wstring& name, const NUIE::Point& position) :
	TransformationNode (name, position)
{

}

void RotationNode::Initialize ()
{
	TransformationNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("angle"), L"Angle", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("axisx"), L"Axis X", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("axisy"), L"Axis Y", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("axisz"), L"Axis Z", NE::ValuePtr (new NE::FloatValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("transformation"), L"Transformation")));
}

NE::ValuePtr RotationNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr angleValue = EvaluateSingleInputSlot (NE::SlotId ("angle"), env);
	NE::ValuePtr axisXValue = EvaluateSingleInputSlot (NE::SlotId ("axisx"), env);
	NE::ValuePtr axisYValue = EvaluateSingleInputSlot (NE::SlotId ("axisy"), env);
	NE::ValuePtr axisZValue = EvaluateSingleInputSlot (NE::SlotId ("axisz"), env);

	if (!NE::IsComplexType<NE::NumberValue> (angleValue) || !NE::IsComplexType<NE::NumberValue> (axisXValue) || !NE::IsComplexType<NE::NumberValue> (axisYValue) || !NE::IsComplexType<NE::NumberValue> (axisZValue)) {
		return nullptr;
	}

	std::shared_ptr<BI::ValueCombinationFeature> valueCombination = BI::GetValueCombinationFeature (this);

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = valueCombination->CombineValues ({angleValue, axisXValue, axisYValue, axisZValue}, [&] (const NE::ValueCombination& combination) {
		float angle = NE::NumberValue::ToFloat (combination.GetValue (0));
		glm::vec3 axis (
			NE::NumberValue::ToFloat (combination.GetValue (1)),
			NE::NumberValue::ToFloat (combination.GetValue (2)),
			NE::NumberValue::ToFloat (combination.GetValue (3))
		);
		if (glm::length (axis) == 0.0f) {
			return false;
		}
		axis = glm::normalize (axis);
		glm::mat4 transformation = glm::rotate (glm::mat4 (1.0f), glm::radians (angle), axis);
		result->Push (NE::ValuePtr (new TransformationValue (transformation)));
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return result;
}

void RotationNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	TransformationNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<RotationNode, NE::FloatValue> (parameterList, L"Angle", NUIE::ParameterType::Float, NE::SlotId ("angle"));
	NUIE::RegisterSlotDefaultValueNodeParameter<RotationNode, NE::FloatValue> (parameterList, L"Axis X", NUIE::ParameterType::Float, NE::SlotId ("axisx"));
	NUIE::RegisterSlotDefaultValueNodeParameter<RotationNode, NE::FloatValue> (parameterList, L"Axis Y", NUIE::ParameterType::Float, NE::SlotId ("axisy"));
	NUIE::RegisterSlotDefaultValueNodeParameter<RotationNode, NE::FloatValue> (parameterList, L"Axis Z", NUIE::ParameterType::Float, NE::SlotId ("axisz"));
}

NE::Stream::Status RotationNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	TransformationNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status RotationNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	TransformationNode::Write (outputStream);
	return outputStream.GetStatus ();
}

ScaleNode::ScaleNode () :
	ScaleNode (L"", NUIE::Point ())
{

}

ScaleNode::ScaleNode (const std::wstring& name, const NUIE::Point& position) :
	TransformationNode (name, position)
{

}

void ScaleNode::Initialize ()
{
	TransformationNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("scalex"), L"Scale X", NE::ValuePtr (new NE::FloatValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("scaley"), L"Scale Y", NE::ValuePtr (new NE::FloatValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("scalez"), L"Scale Z", NE::ValuePtr (new NE::FloatValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("transformation"), L"Transformation")));
}

NE::ValuePtr ScaleNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr scaleXValue = EvaluateSingleInputSlot (NE::SlotId ("scalex"), env);
	NE::ValuePtr scaleYValue = EvaluateSingleInputSlot (NE::SlotId ("scaley"), env);
	NE::ValuePtr scaleZValue = EvaluateSingleInputSlot (NE::SlotId ("scalez"), env);

	if (!NE::IsComplexType<NE::NumberValue> (scaleXValue) || !NE::IsComplexType<NE::NumberValue> (scaleYValue) || !NE::IsComplexType<NE::NumberValue> (scaleZValue)) {
		return nullptr;
	}

	std::shared_ptr<BI::ValueCombinationFeature> valueCombination = BI::GetValueCombinationFeature (this);

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = valueCombination->CombineValues ({scaleXValue, scaleYValue, scaleZValue}, [&] (const NE::ValueCombination& combination) {
		glm::vec3 scale (
			NE::NumberValue::ToFloat (combination.GetValue (0)),
			NE::NumberValue::ToFloat (combination.GetValue (1)),
			NE::NumberValue::ToFloat (combination.GetValue (2))
		);
		if (scale.x <= 0.0f || scale.y <= 0.0f || scale.z <= 0.0f) {
			return false;
		}
		glm::mat4 transformation = glm::scale (glm::mat4 (1.0f), scale);
		result->Push (NE::ValuePtr (new TransformationValue (transformation)));
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return result;
}

void ScaleNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	TransformationNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<ScaleNode, NE::FloatValue> (parameterList, L"Scale X", NUIE::ParameterType::Float, NE::SlotId ("scalex"));
	NUIE::RegisterSlotDefaultValueNodeParameter<ScaleNode, NE::FloatValue> (parameterList, L"Scale Y", NUIE::ParameterType::Float, NE::SlotId ("scaley"));
	NUIE::RegisterSlotDefaultValueNodeParameter<ScaleNode, NE::FloatValue> (parameterList, L"Scale Z", NUIE::ParameterType::Float, NE::SlotId ("scalez"));
}

NE::Stream::Status ScaleNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	TransformationNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ScaleNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	TransformationNode::Write (outputStream);
	return outputStream.GetStatus ();
}

CombinationNode::CombinationNode () :
	CombinationNode (L"", NUIE::Point ())
{

}

CombinationNode::CombinationNode (const std::wstring& name, const NUIE::Point& position) :
	TransformationNode (name, position)
{

}

void CombinationNode::Initialize ()
{
	TransformationNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation1"), L"Transformation 1", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation2"), L"Transformation 2", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("transformation"), L"Transformation")));
}

NE::ValuePtr CombinationNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr transformation1Value = EvaluateSingleInputSlot (NE::SlotId ("transformation1"), env);
	NE::ValuePtr transformation2Value = EvaluateSingleInputSlot (NE::SlotId ("transformation2"), env);

	if (!NE::IsComplexType<TransformationValue> (transformation1Value) || !NE::IsComplexType<TransformationValue> (transformation2Value)) {
		return nullptr;
	}

	std::shared_ptr<BI::ValueCombinationFeature> valueCombination = BI::GetValueCombinationFeature (this);

	NE::ListValuePtr result (new NE::ListValue ());
	valueCombination->CombineValues ({transformation1Value, transformation2Value}, [&] (const NE::ValueCombination& combination) {
		glm::dmat4 transformation1 = TransformationValue::Get (combination.GetValue (0));
		glm::dmat4 transformation2 = TransformationValue::Get (combination.GetValue (1));
		glm::dmat4 transformation = transformation1 * transformation2;
		result->Push (NE::ValuePtr (new TransformationValue (transformation)));
		return true;
	});

	return result;
}

NE::Stream::Status CombinationNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	TransformationNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status CombinationNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	TransformationNode::Write (outputStream);
	return outputStream.GetStatus ();
}

TransformPointNode::TransformPointNode () :
	TransformPointNode (L"", NUIE::Point ())
{

}

TransformPointNode::TransformPointNode (const std::wstring& name, const NUIE::Point& position) :
	TransformationNode (name, position)
{

}

void TransformPointNode::Initialize ()
{
	TransformationNode::Initialize ();
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
	TransformationNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TransformPointNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	TransformationNode::Write (outputStream);
	return outputStream.GetStatus ();
}
