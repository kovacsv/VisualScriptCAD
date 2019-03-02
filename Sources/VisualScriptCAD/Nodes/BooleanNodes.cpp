#include "BooleanNodes.hpp"
#include "NE_SingleValues.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "BI_BuiltInFeatures.hpp"

#include "Basic3DNodeValues.hpp"
#include "ModelEvaluationData.hpp"
#include "TransformationNodes.hpp"
#include "MaterialNode.hpp"

#include "IncludeGLM.hpp"

NE::DynamicSerializationInfo	BooleanNode::serializationInfo (NE::ObjectId ("{558DB17B-A907-4A10-A187-6C317921BB53}"), NE::ObjectVersion (1), BooleanNode::CreateSerializableInstance);

BooleanNode::BooleanNode () :
	BooleanNode (L"", NUIE::Point (), CGALOperations::BooleanOperation::Difference)
{

}

BooleanNode::BooleanNode (const std::wstring& name, const NUIE::Point& position, CGALOperations::BooleanOperation operation) :
	ShapeNode (name, position),
	operation (operation)
{

}

void BooleanNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("a"), L"Shape A", NE::ValuePtr (nullptr), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("b"), L"Shape B", NE::ValuePtr (nullptr), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

NE::ValueConstPtr BooleanNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr transformationValue = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	NE::ValueConstPtr aValue = EvaluateInputSlot (NE::SlotId ("a"), env);
	NE::ValueConstPtr bValue = EvaluateInputSlot (NE::SlotId ("b"), env);

	if (!NE::IsComplexType<TransformationValue> (transformationValue) || !NE::IsComplexType<ShapeValue> (aValue) || !NE::IsComplexType<ShapeValue> (bValue)) {
		return nullptr;
	}
	
	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {transformationValue, aValue, bValue}, [&] (const NE::ValueCombination& combination) {
		Modeler::ShapePtr aShape = ShapeValue::Get (combination.GetValue (1));
		Modeler::ShapePtr bShape = ShapeValue::Get (combination.GetValue (2));
		Modeler::ShapePtr shape = CGALOperations::MeshBooleanOperation (aShape, bShape, operation);
		if (shape == nullptr || !shape->Check ()) {
			return false;
		}
		shape->SetTransformation (TransformationValue::Get (combination.GetValue (0)));
		result->Push (NE::ValuePtr (new ShapeValue (shape)));
		return true;
	});
	
	if (!isValid) {
		return nullptr;
	}
	return result;
}

NE::Stream::Status BooleanNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	int operationInt = 0;
	inputStream.Read (operationInt);
	operation = (CGALOperations::BooleanOperation) operationInt;
	return inputStream.GetStatus ();
}

NE::Stream::Status BooleanNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	outputStream.Write ((int) operation);
	return outputStream.GetStatus ();
}
