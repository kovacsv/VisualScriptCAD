#include "Basic3DTransformationNodes.hpp"
#include "BI_BuiltInFeatures.hpp"

NE::DynamicSerializationInfo	TransformPointNode::serializationInfo (NE::ObjectId ("{8AC8484D-C80C-450B-AF86-B5E915179CED}"), NE::ObjectVersion (1), TransformPointNode::CreateSerializableInstance);

TransformPointNode::TransformPointNode () :
	TransformPointNode (NE::String (), NUIE::Point ())
{

}

TransformPointNode::TransformPointNode (const NE::String& name, const NUIE::Point& position) :
	TransformationMatrixNode (name, position)
{

}

void TransformPointNode::Initialize ()
{
	TransformationMatrixNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("point"), NE::String (L"Point"), NE::ValuePtr (new PointValue (glm::dvec3 (0.0, 0.0, 0.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), NE::String (L"Transformation"), NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("point"), NE::String (L"Point"))));
}

NE::ValueConstPtr TransformPointNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr pointValue = EvaluateInputSlot (NE::SlotId ("point"), env);
	NE::ValueConstPtr transformationValue = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	if (!NE::IsComplexType<CoordinateValue> (pointValue) || !NE::IsComplexType<TransformationValue> (transformationValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	BI::CombineValues (this, {pointValue, transformationValue}, [&] (const NE::ValueCombination& combination) {
		glm::dvec3 point (CoordinateValue::Get (combination.GetValue (0)));
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
	TransformationMatrixNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TransformPointNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	TransformationMatrixNode::Write (outputStream);
	return outputStream.GetStatus ();
}
