#include "TransformShapeNodes.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "Basic3DNodeValues.hpp"

NE::DynamicSerializationInfo	TransformShapeNode::serializationInfo (NE::ObjectId ("{3BA3AD96-09FD-49D2-8299-2143A0A8ECFA}"), NE::ObjectVersion (1), TransformShapeNode::CreateSerializableInstance);

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
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

NE::ValueConstPtr TransformShapeNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr shapeValue = EvaluateInputSlot (NE::SlotId ("shape"), env);
	NE::ValueConstPtr transformationValue = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	if (!NE::IsComplexType<ShapeValue> (shapeValue) || !NE::IsComplexType<TransformationValue> (transformationValue)) {
		return nullptr;
	}

	std::shared_ptr<BI::ValueCombinationFeature> valueCombination = BI::GetValueCombinationFeature (this);

	NE::ListValuePtr result (new NE::ListValue ());
	valueCombination->CombineValues ({shapeValue, transformationValue}, [&] (const NE::ValueCombination& combination) {
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
