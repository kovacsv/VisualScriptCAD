#include "MaterialNode.hpp"

#include "NE_SingleValues.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "ModelEvaluationData.hpp"
#include "GLMReadWrite.hpp"

NE::DynamicSerializationInfo	MaterialValue::serializationInfo (NE::ObjectId ("{C7950BFB-9EB4-4C57-92EE-0E11DDF1F322}"), NE::ObjectVersion (1), MaterialValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	MaterialNode::serializationInfo (NE::ObjectId ("{CA4BCE98-064D-4B80-BBC3-D98C7E3E61B3}"), NE::ObjectVersion (1), MaterialNode::CreateSerializableInstance);

MaterialValue::MaterialValue () :
	MaterialValue (Modeler::DefaultMaterial)
{

}

MaterialValue::MaterialValue (const Modeler::Material& val) :
	NE::GenericValue<Modeler::Material> (val)
{

}

NE::ValuePtr MaterialValue::Clone () const
{
	return NE::ValuePtr (new MaterialValue (val));
}

std::wstring MaterialValue::ToString (const NE::StringConverter& stringConverter) const
{
	std::wstring result;
	const glm::dvec3& color = val.GetColor ();
	result += L"Material (";
	result += stringConverter.IntegerToString (int (color.x * 255.0f)) + L", ";
	result += stringConverter.IntegerToString (int (color.y * 255.0f)) + L", ";
	result += stringConverter.IntegerToString (int (color.z * 255.0f));
	result += L")";
	return result;
}

NE::Stream::Status MaterialValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::GenericValue<Modeler::Material>::Read (inputStream);
	glm::dvec3 color;
	ReadVector (inputStream, color);
	val.SetColor (color);
	return inputStream.GetStatus ();
}

NE::Stream::Status MaterialValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::GenericValue<Modeler::Material>::Write (outputStream);
	const glm::dvec3& color = val.GetColor ();
	WriteVector (outputStream, color);
	return outputStream.GetStatus ();
}

MaterialNode::MaterialNode () :
	MaterialNode (NE::String (), NUIE::Point ())
{

}

MaterialNode::MaterialNode (const NE::String& name, const NUIE::Point& position) :
	BI::BasicUINode (name, position)
{

}

void MaterialNode::Initialize ()
{
	RegisterFeature (BI::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("red"), NE::String (L"Red"), NE::ValuePtr (new NE::IntValue (255)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("green"), NE::String (L"Green"), NE::ValuePtr (new NE::IntValue (255)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("blue"), NE::String (L"Blue"), NE::ValuePtr (new NE::IntValue (255)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("material"), NE::String (L"Material"))));
}

NE::ValueConstPtr MaterialNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr red = EvaluateInputSlot (NE::SlotId ("red"), env);
	NE::ValueConstPtr green = EvaluateInputSlot (NE::SlotId ("green"), env);
	NE::ValueConstPtr blue = EvaluateInputSlot (NE::SlotId ("blue"), env);
	if (!NE::IsComplexType<NE::NumberValue> (red) || !NE::IsComplexType<NE::NumberValue> (green) || !NE::IsComplexType<NE::NumberValue> (blue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	BI::ValueCombinationFeature::CombineValues (this, {red, green, blue}, [&] (const NE::ValueCombination& combination) {
		Modeler::Material material (glm::dvec3 (
			NE::NumberValue::ToFloat (combination.GetValue (0)) / 255.0f,
			NE::NumberValue::ToFloat (combination.GetValue (1)) / 255.0f,
			NE::NumberValue::ToFloat (combination.GetValue (2)) / 255.0f
		));
		result->Push (NE::ValuePtr (new MaterialValue (material)));
		return true;
	});

	return result;
}

void MaterialNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BI::BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<MaterialNode, NE::IntValue> (parameterList, L"Red", NUIE::ParameterType::Integer, NE::SlotId ("red"));
	NUIE::RegisterSlotDefaultValueNodeParameter<MaterialNode, NE::IntValue> (parameterList, L"Green", NUIE::ParameterType::Integer, NE::SlotId ("green"));
	NUIE::RegisterSlotDefaultValueNodeParameter<MaterialNode, NE::IntValue> (parameterList, L"Blue", NUIE::ParameterType::Integer, NE::SlotId ("blue"));
}

NE::Stream::Status MaterialNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BI::BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status MaterialNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BI::BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}
