#include "SubdivisionNodes.hpp"
#include "NE_SingleValues.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "BI_BuiltInFeatures.hpp"

#include "ModelEvaluationData.hpp"
#include "TransformationNodes.hpp"
#include "MaterialNode.hpp"

#include "IncludeGLM.hpp"

NE::DynamicSerializationInfo	SubdivisionNode::serializationInfo (NE::ObjectId ("{65E82D0C-89A1-4672-AFCA-46CD3B1CF71E}"), NE::ObjectVersion (1), SubdivisionNode::CreateSerializableInstance);

SubdivisionNode::SubdivisionNode () :
	SubdivisionNode (L"", NUIE::Point ())
{

}

SubdivisionNode::SubdivisionNode (const std::wstring& name, const NUIE::Point& position) :
	ShapeNode (name, position)
{

}

void SubdivisionNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("material"), L"Material", NE::ValuePtr (new MaterialValue (Modeler::DefaultMaterial)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("shape"), L"Shape", NE::ValuePtr (nullptr), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("steps"), L"Steps", NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

void SubdivisionNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	ShapeNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<SubdivisionNode, NE::IntValue> (parameterList, L"Steps", NUIE::ParameterType::Integer, NE::SlotId ("steps"));
}

NE::ValueConstPtr SubdivisionNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr materialValue = EvaluateSingleInputSlot (NE::SlotId ("material"), env);
	NE::ValueConstPtr transformationValue = EvaluateSingleInputSlot (NE::SlotId ("transformation"), env);
	NE::ValueConstPtr shapeValue = EvaluateSingleInputSlot (NE::SlotId ("shape"), env);
	NE::ValueConstPtr stepsValue = EvaluateSingleInputSlot (NE::SlotId ("steps"), env);

	if (!NE::IsComplexType<MaterialValue> (materialValue) || !NE::IsComplexType<TransformationValue> (transformationValue) || !NE::IsComplexType<ShapeValue> (shapeValue) || !NE::IsComplexType<NE::NumberValue> (stepsValue)) {
		return nullptr;
	}
	
	std::shared_ptr<BI::ValueCombinationFeature> valueCombination = BI::GetValueCombinationFeature (this);
	
	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = valueCombination->CombineValues ({materialValue, transformationValue, shapeValue, stepsValue}, [&] (const NE::ValueCombination& combination) {
		const Modeler::Material& material = MaterialValue::Get (combination.GetValue (0));
		const glm::dmat4& transformation = TransformationValue::Get (combination.GetValue (1));
		Modeler::ShapePtr shape = ShapeValue::Get (combination.GetValue (2));
		int steps = NE::NumberValue::ToInteger (combination.GetValue (3));
		if (steps < 1) {
			return false;
		}
		Modeler::ShapePtr resultShape = CGALOperations::MeshSubdivision (shape, material, steps);
		if (shape == nullptr || !shape->Check ()) {
			return false;
		}
		resultShape->SetTransformation (transformation);
		result->Push (NE::ValuePtr (new ShapeValue (resultShape)));
		return true;
	});
	
	if (!isValid) {
		return nullptr;
	}
	return result;
}

NE::Stream::Status SubdivisionNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status SubdivisionNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	return outputStream.GetStatus ();
}
