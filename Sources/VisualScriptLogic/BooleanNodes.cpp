#include "BooleanNodes.hpp"
#include "NE_SingleValues.hpp"
#include "NUIE_NodeCommonParameters.hpp"

#include "Basic3DNodeValues.hpp"
#include "ModelEvaluationData.hpp"
#include "TransformationNodes.hpp"
#include "MaterialNode.hpp"

#include "IncludeGLM.hpp"

NE::DynamicSerializationInfo	BooleanNode::serializationInfo (NE::ObjectId ("{558DB17B-A907-4A10-A187-6C317921BB53}"), NE::ObjectVersion (1), BooleanNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	UnionNode::serializationInfo (NE::ObjectId ("{F13DD277-9E5F-4CC2-A06A-2194AB5B9BD3}"), NE::ObjectVersion (1), UnionNode::CreateSerializableInstance);

static Modeler::ShapePtr ShapeUnionFromValue (const NE::ValueConstPtr& shapesValue)
{
	if (!NE::IsComplexType<ShapeValue> (shapesValue)) {
		return nullptr;
	}

	std::vector<Modeler::ShapeConstPtr> shapes;
	NE::FlatEnumerate (shapesValue, [&] (const NE::ValueConstPtr& val) {
		shapes.push_back (ShapeValue::Get (val));
	});

	Modeler::ShapePtr shape = CGALOperations::ShapeUnion (shapes);
	if (shape == nullptr || !shape->Check ()) {
		return nullptr;
	}
	return shape;
}

BooleanNode::BooleanNode () :
	BooleanNode (NE::String (), NUIE::Point (), Operation::Difference)
{

}

BooleanNode::BooleanNode (const NE::String& name, const NUIE::Point& position, Operation operation) :
	ShapeNode (name, position),
	operation (operation)
{

}

void BooleanNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), NE::String (L"Transformation"), NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("ashapes"), NE::String (L"Shapes A"), NE::ValuePtr (nullptr), NE::OutputSlotConnectionMode::Multiple)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("bshapes"), NE::String (L"Shapes B"), NE::ValuePtr (nullptr), NE::OutputSlotConnectionMode::Multiple)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), NE::String (L"Shape"))));
}

NE::ValueConstPtr BooleanNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr transformationValue = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	NE::ValueConstPtr aShapesValue = NE::FlattenValue (EvaluateInputSlot (NE::SlotId ("ashapes"), env));
	NE::ValueConstPtr bShapesValue = NE::FlattenValue (EvaluateInputSlot (NE::SlotId ("bshapes"), env));
	if (!NE::IsComplexType<TransformationValue> (transformationValue) || !NE::IsComplexType<ShapeValue> (aShapesValue) || !NE::IsComplexType<ShapeValue> (bShapesValue)) {
		return nullptr;
	}

	Modeler::ShapePtr aShape = ShapeUnionFromValue (aShapesValue);
	if (aShape == nullptr || !aShape->Check ()) {
		return nullptr;
	}

	Modeler::ShapePtr bShape = ShapeUnionFromValue (bShapesValue);
	if (bShape == nullptr || !bShape->Check ()) {
		return nullptr;
	}

	Modeler::ShapePtr shape = nullptr;
	if (operation == Operation::Difference) {
		shape = CGALOperations::ShapeDifference (aShape, bShape);
	} else if (operation == Operation::Intersection) {
		shape = CGALOperations::ShapeIntersection (aShape, bShape);
	}
	if (shape == nullptr || !shape->Check ()) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	NE::FlatEnumerate (transformationValue, [&] (const NE::ValueConstPtr& val) {
		Modeler::ShapePtr transformed = shape->Transform (TransformationValue::Get (val));
		result->Push (NE::ValuePtr (new ShapeValue (transformed)));
	});	
	return result;
}

NE::Stream::Status BooleanNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	int operationInt = 0;
	inputStream.Read (operationInt);
	operation = (Operation) operationInt;
	return inputStream.GetStatus ();
}

NE::Stream::Status BooleanNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	outputStream.Write ((int) operation);
	return outputStream.GetStatus ();
}

UnionNode::UnionNode () :
	UnionNode (NE::String (), NUIE::Point ())
{

}

UnionNode::UnionNode (const NE::String& name, const NUIE::Point& position) :
	ShapeNode (name, position)
{

}

void UnionNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), NE::String (L"Transformation"), NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("shapes"), NE::String (L"Shapes"), NE::ValuePtr (nullptr), NE::OutputSlotConnectionMode::Multiple)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), NE::String (L"Shape"))));
}

NE::ValueConstPtr UnionNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr transformationValue = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	NE::ValueConstPtr shapesValue = NE::FlattenValue (EvaluateInputSlot (NE::SlotId ("shapes"), env));
	if (!NE::IsComplexType<TransformationValue> (transformationValue) || !NE::IsComplexType<ShapeValue> (shapesValue)) {
		return nullptr;
	}

	Modeler::ShapePtr shape = ShapeUnionFromValue (shapesValue);
	if (shape == nullptr || !shape->Check ()) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	NE::FlatEnumerate (transformationValue, [&] (const NE::ValueConstPtr& val) {
		Modeler::ShapePtr transformed = shape->Transform (TransformationValue::Get (val));
		result->Push (NE::ValuePtr (new ShapeValue (transformed)));
	});	
	return result;
}

NE::Stream::Status UnionNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status UnionNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	return outputStream.GetStatus ();
}
