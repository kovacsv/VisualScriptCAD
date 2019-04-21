#include "TransformationNodes.hpp"
#include "NE_SingleValues.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "Geometry.hpp"
#include "GLMReadWrite.hpp"

NE::SerializationInfo			TransformationMatrixNode::serializationInfo (NE::ObjectVersion (1));
NE::DynamicSerializationInfo	TranslationMatrixNode::serializationInfo (NE::ObjectId ("{DDB65DA8-1E2C-47EF-9658-96819C29259D}"), NE::ObjectVersion (1), TranslationMatrixNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	TranslationMatrixXYZNode::serializationInfo (NE::ObjectId ("{7F1B6F2F-2DE7-48FC-A926-5137FB9CFFF5}"), NE::ObjectVersion (1), TranslationMatrixXYZNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	RotationMatrixNode::serializationInfo (NE::ObjectId ("{2F11941E-6B80-49A0-A553-BD230AC0336C}"), NE::ObjectVersion (1), RotationMatrixNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	ScaleMatrixNode::serializationInfo (NE::ObjectId ("{9E070858-8AC9-42DE-95A6-0C2AB6807426}"), NE::ObjectVersion (1), ScaleMatrixNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	MatrixCombinationNode::serializationInfo (NE::ObjectId ("{E6FC2758-00D6-47F6-B321-AE46ECEA66C5}"), NE::ObjectVersion (1), MatrixCombinationNode::CreateSerializableInstance);

TransformationMatrixNode::TransformationMatrixNode () :
	TransformationMatrixNode (L"", NUIE::Point ())
{

}

TransformationMatrixNode::TransformationMatrixNode (const std::wstring& name, const NUIE::Point& position) :
	BI::BasicUINode (name, position)
{

}

void TransformationMatrixNode::Initialize ()
{
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
}

NE::Stream::Status TransformationMatrixNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BI::BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TransformationMatrixNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BI::BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

TranslationMatrixNode::TranslationMatrixNode () :
	TranslationMatrixNode (L"", NUIE::Point ())
{

}

TranslationMatrixNode::TranslationMatrixNode (const std::wstring& name, const NUIE::Point& position) :
	TransformationMatrixNode (name, position)
{

}

void TranslationMatrixNode::Initialize ()
{
	TransformationMatrixNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("offset"), L"Offset", NE::ValuePtr (new PointValue (glm::vec3 (0.0f))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("transformation"), L"Transformation")));
}

NE::ValueConstPtr TranslationMatrixNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr offsetValue = EvaluateInputSlot (NE::SlotId ("offset"), env);
	if (!NE::IsComplexType<CoordinateValue> (offsetValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	BI::CombineValues (this, {offsetValue}, [&] (const NE::ValueCombination& combination) {
		glm::vec3 offset = CoordinateValue::Get (combination.GetValue (0));
		glm::mat4 transformation = glm::translate (glm::mat4 (1.0f), offset);
		result->Push (NE::ValuePtr (new TransformationValue (transformation)));
		return true;
	});

	return result;
}

NE::Stream::Status TranslationMatrixNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	TransformationMatrixNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TranslationMatrixNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	TransformationMatrixNode::Write (outputStream);
	return outputStream.GetStatus ();
}

TranslationMatrixXYZNode::TranslationMatrixXYZNode () :
	TranslationMatrixXYZNode (L"", NUIE::Point ())
{

}

TranslationMatrixXYZNode::TranslationMatrixXYZNode (const std::wstring& name, const NUIE::Point& position) :
	TransformationMatrixNode (name, position)
{

}

void TranslationMatrixXYZNode::Initialize ()
{
	TransformationMatrixNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("offsetx"), L"Offset X", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("offsety"), L"Offset Y", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("offsetz"), L"Offset Z", NE::ValuePtr (new NE::FloatValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("transformation"), L"Transformation")));
}

NE::ValueConstPtr TranslationMatrixXYZNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr offsetXValue = EvaluateInputSlot (NE::SlotId ("offsetx"), env);
	NE::ValueConstPtr offsetYValue = EvaluateInputSlot (NE::SlotId ("offsety"), env);
	NE::ValueConstPtr offsetZValue = EvaluateInputSlot (NE::SlotId ("offsetz"), env);
	if (!NE::IsComplexType<NE::NumberValue> (offsetXValue) || !NE::IsComplexType<NE::NumberValue> (offsetYValue) || !NE::IsComplexType<NE::NumberValue> (offsetZValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	BI::CombineValues (this, {offsetXValue, offsetYValue, offsetZValue}, [&] (const NE::ValueCombination& combination) {
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

void TranslationMatrixXYZNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	TransformationMatrixNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<TranslationMatrixXYZNode, NE::FloatValue> (parameterList, L"Offset X", NUIE::ParameterType::Float, NE::SlotId ("offsetx"));
	NUIE::RegisterSlotDefaultValueNodeParameter<TranslationMatrixXYZNode, NE::FloatValue> (parameterList, L"Offset Y", NUIE::ParameterType::Float, NE::SlotId ("offsety"));
	NUIE::RegisterSlotDefaultValueNodeParameter<TranslationMatrixXYZNode, NE::FloatValue> (parameterList, L"Offset Z", NUIE::ParameterType::Float, NE::SlotId ("offsetz"));
}

NE::Stream::Status TranslationMatrixXYZNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	TransformationMatrixNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TranslationMatrixXYZNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	TransformationMatrixNode::Write (outputStream);
	return outputStream.GetStatus ();
}

RotationMatrixNode::RotationMatrixNode () :
	RotationMatrixNode (L"", NUIE::Point ())
{

}

RotationMatrixNode::RotationMatrixNode (const std::wstring& name, const NUIE::Point& position) :
	TransformationMatrixNode (name, position)
{

}

void RotationMatrixNode::Initialize ()
{
	TransformationMatrixNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("angle"), L"Angle", NE::ValuePtr (new NE::FloatValue (0.0f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("origin"), L"Origin", NE::ValuePtr (new PointValue (glm::vec3 (0.0f, 0.0f, 0.0f))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("axis"), L"Axis", NE::ValuePtr (new PointValue (glm::vec3 (0.0f, 0.0f, 1.0f))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("transformation"), L"Transformation")));
}

NE::ValueConstPtr RotationMatrixNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr angleValue = EvaluateInputSlot (NE::SlotId ("angle"), env);
	NE::ValueConstPtr originValue = EvaluateInputSlot (NE::SlotId ("origin"), env);
	NE::ValueConstPtr axisValue = EvaluateInputSlot (NE::SlotId ("axis"), env);
	if (!NE::IsComplexType<NE::NumberValue> (angleValue) || !NE::IsComplexType<CoordinateValue> (originValue) || !NE::IsComplexType<CoordinateValue> (axisValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {angleValue, originValue, axisValue}, [&] (const NE::ValueCombination& combination) {
		float angle = NE::NumberValue::ToFloat (combination.GetValue (0));
		glm::vec3 origin = CoordinateValue::Get (combination.GetValue (1));
		glm::vec3 axis = CoordinateValue::Get (combination.GetValue (2));
		if (Geometry::IsEqual (glm::length (axis), 0.0f)) {
			return false;
		}
		glm::mat4 offset = glm::translate (glm::mat4 (1.0f), -origin);
		glm::mat4 rotation = glm::rotate (glm::mat4 (1.0f), glm::radians (angle), axis);
		glm::mat4 offsetBack = glm::translate (glm::mat4 (1.0f), origin);
		glm::mat4 transformation = offsetBack * rotation * offset;
		result->Push (NE::ValuePtr (new TransformationValue (transformation)));
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return result;
}

void RotationMatrixNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	TransformationMatrixNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<RotationMatrixNode, NE::FloatValue> (parameterList, L"Angle", NUIE::ParameterType::Float, NE::SlotId ("angle"));
}

NE::Stream::Status RotationMatrixNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	TransformationMatrixNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status RotationMatrixNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	TransformationMatrixNode::Write (outputStream);
	return outputStream.GetStatus ();
}

ScaleMatrixNode::ScaleMatrixNode () :
	ScaleMatrixNode (L"", NUIE::Point ())
{

}

ScaleMatrixNode::ScaleMatrixNode (const std::wstring& name, const NUIE::Point& position) :
	TransformationMatrixNode (name, position)
{

}

void ScaleMatrixNode::Initialize ()
{
	TransformationMatrixNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("scalex"), L"Scale X", NE::ValuePtr (new NE::FloatValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("scaley"), L"Scale Y", NE::ValuePtr (new NE::FloatValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("scalez"), L"Scale Z", NE::ValuePtr (new NE::FloatValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("transformation"), L"Transformation")));
}

NE::ValueConstPtr ScaleMatrixNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr scaleXValue = EvaluateInputSlot (NE::SlotId ("scalex"), env);
	NE::ValueConstPtr scaleYValue = EvaluateInputSlot (NE::SlotId ("scaley"), env);
	NE::ValueConstPtr scaleZValue = EvaluateInputSlot (NE::SlotId ("scalez"), env);

	if (!NE::IsComplexType<NE::NumberValue> (scaleXValue) || !NE::IsComplexType<NE::NumberValue> (scaleYValue) || !NE::IsComplexType<NE::NumberValue> (scaleZValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {scaleXValue, scaleYValue, scaleZValue}, [&] (const NE::ValueCombination& combination) {
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

void ScaleMatrixNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	TransformationMatrixNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<ScaleMatrixNode, NE::FloatValue> (parameterList, L"Scale X", NUIE::ParameterType::Float, NE::SlotId ("scalex"));
	NUIE::RegisterSlotDefaultValueNodeParameter<ScaleMatrixNode, NE::FloatValue> (parameterList, L"Scale Y", NUIE::ParameterType::Float, NE::SlotId ("scaley"));
	NUIE::RegisterSlotDefaultValueNodeParameter<ScaleMatrixNode, NE::FloatValue> (parameterList, L"Scale Z", NUIE::ParameterType::Float, NE::SlotId ("scalez"));
}

NE::Stream::Status ScaleMatrixNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	TransformationMatrixNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ScaleMatrixNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	TransformationMatrixNode::Write (outputStream);
	return outputStream.GetStatus ();
}

MatrixCombinationNode::MatrixCombinationNode () :
	MatrixCombinationNode (L"", NUIE::Point ())
{

}

MatrixCombinationNode::MatrixCombinationNode (const std::wstring& name, const NUIE::Point& position) :
	TransformationMatrixNode (name, position)
{

}

void MatrixCombinationNode::Initialize ()
{
	TransformationMatrixNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation1"), L"Transformation 1", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation2"), L"Transformation 2", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("transformation"), L"Transformation")));
}

NE::ValueConstPtr MatrixCombinationNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr transformation1Value = EvaluateInputSlot (NE::SlotId ("transformation1"), env);
	NE::ValueConstPtr transformation2Value = EvaluateInputSlot (NE::SlotId ("transformation2"), env);

	if (!NE::IsComplexType<TransformationValue> (transformation1Value) || !NE::IsComplexType<TransformationValue> (transformation2Value)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	BI::CombineValues (this, {transformation1Value, transformation2Value}, [&] (const NE::ValueCombination& combination) {
		glm::dmat4 transformation1 = TransformationValue::Get (combination.GetValue (0));
		glm::dmat4 transformation2 = TransformationValue::Get (combination.GetValue (1));
		glm::dmat4 transformation = transformation2 * transformation1;
		result->Push (NE::ValuePtr (new TransformationValue (transformation)));
		return true;
	});

	return result;
}

NE::Stream::Status MatrixCombinationNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	TransformationMatrixNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status MatrixCombinationNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	TransformationMatrixNode::Write (outputStream);
	return outputStream.GetStatus ();
}
