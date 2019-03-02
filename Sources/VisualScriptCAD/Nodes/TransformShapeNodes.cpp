#include "TransformShapeNodes.hpp"
#include "NE_SingleValues.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "Basic3DNodeValues.hpp"
#include "Geometry.hpp"

NE::DynamicSerializationInfo	TranslateShapeNode::serializationInfo (NE::ObjectId ("{AC4C90C2-5B10-45DB-9C62-A118BA993B2F}"), NE::ObjectVersion (1), TranslateShapeNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	RotateShapeNode::serializationInfo (NE::ObjectId ("{92A0FAB9-A100-4920-A2AC-9D8289F4C4B8}"), NE::ObjectVersion (1), RotateShapeNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	TransformShapeNode::serializationInfo (NE::ObjectId ("{3BA3AD96-09FD-49D2-8299-2143A0A8ECFA}"), NE::ObjectVersion (1), TransformShapeNode::CreateSerializableInstance);

TranslateShapeNode::TranslateShapeNode () :
	TranslateShapeNode (L"", NUIE::Point ())
{

}

TranslateShapeNode::TranslateShapeNode (const std::wstring& name, const NUIE::Point& position) :
	ShapeNode (name, position)
{

}

void TranslateShapeNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("shape"), L"Shape", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("offset"), L"Offset", NE::ValuePtr (new PointValue (glm::vec3 (0.0f))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

NE::ValueConstPtr TranslateShapeNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr shapeValue = EvaluateInputSlot (NE::SlotId ("shape"), env);
	NE::ValueConstPtr offsetValue = EvaluateInputSlot (NE::SlotId ("offset"), env);
	if (!NE::IsComplexType<ShapeValue> (shapeValue) || !NE::IsComplexType<CoordinateValue> (offsetValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	BI::CombineValues (this, {shapeValue, offsetValue}, [&] (const NE::ValueCombination& combination) {
		Modeler::ShapePtr shape (ShapeValue::Get (combination.GetValue (0)));
		glm::vec3 direction = CoordinateValue::Get (combination.GetValue (1));
		glm::mat4 transformation = glm::translate (glm::mat4 (1.0f), direction);
		Modeler::ShapePtr transformed = shape->Transform (transformation);
		result->Push (NE::ValuePtr (new ShapeValue (transformed)));
		return true;
	});

	return result;
}

NE::Stream::Status TranslateShapeNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TranslateShapeNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

RotateShapeNode::RotateShapeNode () :
	RotateShapeNode (L"", NUIE::Point ())
{

}

RotateShapeNode::RotateShapeNode (const std::wstring& name, const NUIE::Point& position) :
	ShapeNode (name, position)
{

}

void RotateShapeNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("shape"), L"Shape", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("angle"), L"Angle", NE::ValuePtr (new NE::FloatValue (0.0f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("origin"), L"Origin", NE::ValuePtr (new PointValue (glm::vec3 (0.0f, 0.0f, 0.0f))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("axis"), L"Axis", NE::ValuePtr (new PointValue (glm::vec3 (0.0f, 0.0f, 1.0f))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

void RotateShapeNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	ShapeNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<RotateShapeNode, NE::FloatValue> (parameterList, L"Angle", NUIE::ParameterType::Float, NE::SlotId ("angle"));
}

NE::ValueConstPtr RotateShapeNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr shapeValue = EvaluateInputSlot (NE::SlotId ("shape"), env);
	NE::ValueConstPtr angleValue = EvaluateInputSlot (NE::SlotId ("angle"), env);
	NE::ValueConstPtr originValue = EvaluateInputSlot (NE::SlotId ("origin"), env);
	NE::ValueConstPtr axisValue = EvaluateInputSlot (NE::SlotId ("axis"), env);
	if (!NE::IsComplexType<ShapeValue> (shapeValue) || !NE::IsComplexType<NE::NumberValue> (angleValue) || !NE::IsComplexType<CoordinateValue> (originValue) || !NE::IsComplexType<CoordinateValue> (axisValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {shapeValue, angleValue, originValue, axisValue}, [&] (const NE::ValueCombination& combination) {
		Modeler::ShapePtr shape (ShapeValue::Get (combination.GetValue (0)));
		float angle = NE::NumberValue::ToFloat (combination.GetValue (1));
		glm::vec3 origin = CoordinateValue::Get (combination.GetValue (2));
		glm::vec3 axis = CoordinateValue::Get (combination.GetValue (3));
		if (Geometry::IsEqual (glm::length (axis), 0.0f)) {
			return false;
		}
		glm::mat4 offset = glm::translate (glm::mat4 (1.0f), -origin);
		glm::mat4 rotation = glm::rotate (glm::mat4 (1.0f), glm::radians (angle), axis);
		glm::mat4 offsetBack = glm::translate (glm::mat4 (1.0f), origin);
		glm::mat4 transformation = offsetBack * rotation * offset;
		Modeler::ShapePtr transformed = shape->Transform (transformation);
		result->Push (NE::ValuePtr (new ShapeValue (transformed)));
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return result;
}

NE::Stream::Status RotateShapeNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status RotateShapeNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

TransformShapeNode::TransformShapeNode () :
	TransformShapeNode (L"", NUIE::Point ())
{

}

TransformShapeNode::TransformShapeNode (const std::wstring& name, const NUIE::Point& position) :
	ShapeNode (name, position)
{

}

void TransformShapeNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("shape"), L"Shape", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::mat4 (1.0f))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

NE::ValueConstPtr TransformShapeNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr shapeValue = EvaluateInputSlot (NE::SlotId ("shape"), env);
	NE::ValueConstPtr transformationValue = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	if (!NE::IsComplexType<ShapeValue> (shapeValue) || !NE::IsComplexType<TransformationValue> (transformationValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	BI::CombineValues (this, {shapeValue, transformationValue}, [&] (const NE::ValueCombination& combination) {
		Modeler::ShapePtr shape (ShapeValue::Get (combination.GetValue (0)));
		glm::dmat4 transformation (TransformationValue::Get (combination.GetValue (1)));
		Modeler::ShapePtr transformed = shape->Transform (transformation);
		result->Push (NE::ValuePtr (new ShapeValue (transformed)));
		return true;
	});

	return result;
}

NE::Stream::Status TransformShapeNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TransformShapeNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	return outputStream.GetStatus ();
}
