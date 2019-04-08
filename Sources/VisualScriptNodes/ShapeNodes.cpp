#include "ShapeNodes.hpp"
#include "NE_SingleValues.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "BI_BuiltInFeatures.hpp"

#include "ModelEvaluationData.hpp"
#include "TransformationNodes.hpp"
#include "MaterialNode.hpp"

#include "IncludeGLM.hpp"
#include "BasicShapes.hpp"

NE::DynamicSerializationInfo	BoxNode::serializationInfo (NE::ObjectId ("{9C29EF6D-AD3B-466C-8574-95B82D4EC0D4}"), NE::ObjectVersion (1), BoxNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	BoxShellNode::serializationInfo (NE::ObjectId ("{CCCCF9ED-6B97-4EAA-BDCF-D2A982B7D37A}"), NE::ObjectVersion (1), BoxShellNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	CylinderNode::serializationInfo (NE::ObjectId ("{6C457800-788A-4747-A2D1-54158EFB2794}"), NE::ObjectVersion (1), CylinderNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	CylinderShellNode::serializationInfo (NE::ObjectId ("{73164BBB-3971-46FB-9D17-E6BCA5814420}"), NE::ObjectVersion (1), CylinderShellNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	ConeNode::serializationInfo (NE::ObjectId ("{586F0D6E-CBEB-4C19-8B9E-7358E3E75FD2}"), NE::ObjectVersion (1), ConeNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	SphereNode::serializationInfo (NE::ObjectId ("{686712CE-BF1B-438E-8C0A-B687A158A0BB}"), NE::ObjectVersion (1), SphereNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	TorusNode::serializationInfo (NE::ObjectId ("{E17CF103-A4B6-4498-BB7E-7A566C1F7D26}"), NE::ObjectVersion (1), TorusNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	PlatonicNode::serializationInfo (NE::ObjectId ("{F7321055-D370-4468-A895-32FA3AD1BF40}"), NE::ObjectVersion (1), PlatonicNode::CreateSerializableInstance);

static bool IsSmooth (int segmentation)
{
	static const int minSmoothSegmentation = 10;
	return segmentation >= minSmoothSegmentation;
}

BoxNode::BoxNode () :
	BoxNode (L"", NUIE::Point ())
{

}

BoxNode::BoxNode (const std::wstring& name, const NUIE::Point& position) :
	ShapeNode (name, position)
{

}

void BoxNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("material"), L"Material", NE::ValuePtr (new MaterialValue (Modeler::DefaultMaterial)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("a"), L"Size A", NE::ValuePtr (new NE::FloatValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("b"), L"Size B", NE::ValuePtr (new NE::FloatValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("c"), L"Size C", NE::ValuePtr (new NE::FloatValue (1.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

NE::ValueConstPtr BoxNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr material = EvaluateInputSlot (NE::SlotId ("material"), env);
	NE::ValueConstPtr transformation = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	NE::ValueConstPtr aValue = EvaluateInputSlot (NE::SlotId ("a"), env);
	NE::ValueConstPtr bValue = EvaluateInputSlot (NE::SlotId ("b"), env);
	NE::ValueConstPtr cValue = EvaluateInputSlot (NE::SlotId ("c"), env);

	if (!NE::IsComplexType<MaterialValue> (material) || !NE::IsComplexType<TransformationValue> (transformation) || !NE::IsComplexType<NE::NumberValue> (aValue) || !NE::IsComplexType<NE::NumberValue> (bValue) || !NE::IsComplexType<NE::NumberValue> (cValue)) {
		return nullptr;
	}
	
	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {material, transformation, aValue, bValue, cValue}, [&] (const NE::ValueCombination& combination) {
		Modeler::ShapePtr shape (new Modeler::BoxShape (
			MaterialValue::Get (combination.GetValue (0)),
			TransformationValue::Get (combination.GetValue (1)),
			NE::NumberValue::ToDouble (combination.GetValue (2)),
			NE::NumberValue::ToDouble (combination.GetValue (3)),
			NE::NumberValue::ToDouble (combination.GetValue (4))
		));
		if (!shape->Check ()) {
			return false;
		}
		result->Push (NE::ValuePtr (new ShapeValue (shape)));
		return true;
	});
	
	if (!isValid) {
		return nullptr;
	}
	return result;
}

void BoxNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	ShapeNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<BoxNode, NE::FloatValue> (parameterList, L"Size A", NUIE::ParameterType::Float, NE::SlotId ("a"));
	NUIE::RegisterSlotDefaultValueNodeParameter<BoxNode, NE::FloatValue> (parameterList, L"Size B", NUIE::ParameterType::Float, NE::SlotId ("b"));
	NUIE::RegisterSlotDefaultValueNodeParameter<BoxNode, NE::FloatValue> (parameterList, L"Size C", NUIE::ParameterType::Float, NE::SlotId ("c"));
}

NE::Stream::Status BoxNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status BoxNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

BoxShellNode::BoxShellNode () :
	BoxShellNode (L"", NUIE::Point ())
{

}

BoxShellNode::BoxShellNode (const std::wstring& name, const NUIE::Point& position) :
	ShapeNode (name, position)
{

}

void BoxShellNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("material"), L"Material", NE::ValuePtr (new MaterialValue (Modeler::DefaultMaterial)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("a"), L"Size A", NE::ValuePtr (new NE::FloatValue (1.0f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("b"), L"Size B", NE::ValuePtr (new NE::FloatValue (1.0f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("c"), L"Size C", NE::ValuePtr (new NE::FloatValue (1.0f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("thickness"), L"Thickness", NE::ValuePtr (new NE::FloatValue (0.1f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

NE::ValueConstPtr BoxShellNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr material = EvaluateInputSlot (NE::SlotId ("material"), env);
	NE::ValueConstPtr transformation = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	NE::ValueConstPtr aValue = EvaluateInputSlot (NE::SlotId ("a"), env);
	NE::ValueConstPtr bValue = EvaluateInputSlot (NE::SlotId ("b"), env);
	NE::ValueConstPtr cValue = EvaluateInputSlot (NE::SlotId ("c"), env);
	NE::ValueConstPtr thicknessValue = EvaluateInputSlot (NE::SlotId ("thickness"), env);

	if (!NE::IsComplexType<MaterialValue> (material) || !NE::IsComplexType<TransformationValue> (transformation) || !NE::IsComplexType<NE::NumberValue> (aValue) || !NE::IsComplexType<NE::NumberValue> (bValue) || !NE::IsComplexType<NE::NumberValue> (cValue) || !NE::IsComplexType<NE::NumberValue> (thicknessValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {material, transformation, aValue, bValue, cValue, thicknessValue}, [&] (const NE::ValueCombination& combination) {
		Modeler::ShapePtr shape (new Modeler::BoxShellShape (
			MaterialValue::Get (combination.GetValue (0)),
			TransformationValue::Get (combination.GetValue (1)),
			NE::NumberValue::ToDouble (combination.GetValue (2)),
			NE::NumberValue::ToDouble (combination.GetValue (3)),
			NE::NumberValue::ToDouble (combination.GetValue (4)),
			NE::NumberValue::ToDouble (combination.GetValue (5))
		));
		if (!shape->Check ()) {
			return false;
		}
		result->Push (NE::ValuePtr (new ShapeValue (shape)));
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return result;
}

void BoxShellNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	ShapeNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<BoxShellNode, NE::FloatValue> (parameterList, L"Size A", NUIE::ParameterType::Float, NE::SlotId ("a"));
	NUIE::RegisterSlotDefaultValueNodeParameter<BoxShellNode, NE::FloatValue> (parameterList, L"Size B", NUIE::ParameterType::Float, NE::SlotId ("b"));
	NUIE::RegisterSlotDefaultValueNodeParameter<BoxShellNode, NE::FloatValue> (parameterList, L"Size C", NUIE::ParameterType::Float, NE::SlotId ("c"));
	NUIE::RegisterSlotDefaultValueNodeParameter<BoxShellNode, NE::FloatValue> (parameterList, L"Thickness", NUIE::ParameterType::Float, NE::SlotId ("thickness"));
}

NE::Stream::Status BoxShellNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status BoxShellNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

CylinderNode::CylinderNode () :
	CylinderNode (L"", NUIE::Point ())
{

}

CylinderNode::CylinderNode (const std::wstring& name, const NUIE::Point& position) :
	ShapeNode (name, position)
{

}

void CylinderNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("material"), L"Material", NE::ValuePtr (new MaterialValue (Modeler::DefaultMaterial)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("radius"), L"Radius", NE::ValuePtr (new NE::FloatValue (0.5f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("height"), L"Height", NE::ValuePtr (new NE::FloatValue (1.0f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("segmentation"), L"Segmentation", NE::ValuePtr (new NE::IntValue (25)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

NE::ValueConstPtr CylinderNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr material = EvaluateInputSlot (NE::SlotId ("material"), env);
	NE::ValueConstPtr transformation = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	NE::ValueConstPtr radiusValue = EvaluateInputSlot (NE::SlotId ("radius"), env);
	NE::ValueConstPtr heightValue = EvaluateInputSlot (NE::SlotId ("height"), env);
	NE::ValueConstPtr segmentationValue = EvaluateInputSlot (NE::SlotId ("segmentation"), env);

	if (!NE::IsComplexType<MaterialValue> (material) || !NE::IsComplexType<TransformationValue> (transformation) || !NE::IsComplexType<NE::NumberValue> (radiusValue) || !NE::IsComplexType<NE::NumberValue> (heightValue) || !NE::IsComplexType<NE::NumberValue> (segmentationValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {material, transformation, radiusValue, heightValue, segmentationValue}, [&] (const NE::ValueCombination& combination) {
		int segmentation = NE::NumberValue::ToInteger (combination.GetValue (4));
		Modeler::ShapePtr shape (new Modeler::CylinderShape (
			MaterialValue::Get (combination.GetValue (0)),
			TransformationValue::Get (combination.GetValue (1)),
			NE::NumberValue::ToDouble (combination.GetValue (2)),
			NE::NumberValue::ToDouble (combination.GetValue (3)),
			segmentation,
			IsSmooth (segmentation)
		));
		if (!shape->Check ()) {
			return false;
		}
		result->Push (NE::ValuePtr (new ShapeValue (shape)));
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return result;
}

void CylinderNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	ShapeNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<CylinderNode, NE::FloatValue> (parameterList, L"Radius", NUIE::ParameterType::Float, NE::SlotId ("radius"));
	NUIE::RegisterSlotDefaultValueNodeParameter<CylinderNode, NE::FloatValue> (parameterList, L"Height", NUIE::ParameterType::Float, NE::SlotId ("height"));
	NUIE::RegisterSlotDefaultValueNodeParameter<CylinderNode, NE::IntValue> (parameterList, L"Segmentation", NUIE::ParameterType::Integer, NE::SlotId ("segmentation"));
}

NE::Stream::Status CylinderNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status CylinderNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

CylinderShellNode::CylinderShellNode () :
	CylinderShellNode (L"", NUIE::Point ())
{

}

CylinderShellNode::CylinderShellNode (const std::wstring& name, const NUIE::Point& position) :
	ShapeNode (name, position)
{

}

void CylinderShellNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("material"), L"Material", NE::ValuePtr (new MaterialValue (Modeler::DefaultMaterial)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("radius"), L"Radius", NE::ValuePtr (new NE::FloatValue (0.5f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("height"), L"Height", NE::ValuePtr (new NE::FloatValue (1.0f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("thickness"), L"Thickness", NE::ValuePtr (new NE::FloatValue (0.1f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("segmentation"), L"Segmentation", NE::ValuePtr (new NE::IntValue (25)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

NE::ValueConstPtr CylinderShellNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr material = EvaluateInputSlot (NE::SlotId ("material"), env);
	NE::ValueConstPtr transformation = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	NE::ValueConstPtr radiusValue = EvaluateInputSlot (NE::SlotId ("radius"), env);
	NE::ValueConstPtr heightValue = EvaluateInputSlot (NE::SlotId ("height"), env);
	NE::ValueConstPtr thicknessValue = EvaluateInputSlot (NE::SlotId ("thickness"), env);
	NE::ValueConstPtr segmentationValue = EvaluateInputSlot (NE::SlotId ("segmentation"), env);

	if (!NE::IsComplexType<MaterialValue> (material) || !NE::IsComplexType<TransformationValue> (transformation) || !NE::IsComplexType<NE::NumberValue> (radiusValue) || !NE::IsComplexType<NE::NumberValue> (heightValue) || !NE::IsComplexType<NE::NumberValue> (thicknessValue) || !NE::IsComplexType<NE::NumberValue> (segmentationValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {material, transformation, radiusValue, heightValue, thicknessValue, segmentationValue}, [&] (const NE::ValueCombination& combination) {
		int segmentation = NE::NumberValue::ToInteger (combination.GetValue (5));
		Modeler::ShapePtr shape (new Modeler::CylinderShellShape (
			MaterialValue::Get (combination.GetValue (0)),
			TransformationValue::Get (combination.GetValue (1)),
			NE::NumberValue::ToDouble (combination.GetValue (2)),
			NE::NumberValue::ToDouble (combination.GetValue (3)),
			segmentation,
			IsSmooth (segmentation),
			NE::NumberValue::ToDouble (combination.GetValue (4))
		));
		if (!shape->Check ()) {
			return false;
		}
		result->Push (NE::ValuePtr (new ShapeValue (shape)));
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return result;
}

void CylinderShellNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	ShapeNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<CylinderShellNode, NE::FloatValue> (parameterList, L"Radius", NUIE::ParameterType::Float, NE::SlotId ("radius"));
	NUIE::RegisterSlotDefaultValueNodeParameter<CylinderShellNode, NE::FloatValue> (parameterList, L"Height", NUIE::ParameterType::Float, NE::SlotId ("height"));
	NUIE::RegisterSlotDefaultValueNodeParameter<CylinderShellNode, NE::FloatValue> (parameterList, L"Thickness", NUIE::ParameterType::Float, NE::SlotId ("thickness"));
	NUIE::RegisterSlotDefaultValueNodeParameter<CylinderShellNode, NE::IntValue> (parameterList, L"Segmentation", NUIE::ParameterType::Integer, NE::SlotId ("segmentation"));
}

NE::Stream::Status CylinderShellNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status CylinderShellNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

ConeNode::ConeNode () :
	ConeNode (L"", NUIE::Point ())
{

}

ConeNode::ConeNode (const std::wstring& name, const NUIE::Point& position) :
	ShapeNode (name, position)
{

}

void ConeNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("material"), L"Material", NE::ValuePtr (new MaterialValue (Modeler::DefaultMaterial)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("topradius"), L"Top Radius", NE::ValuePtr (new NE::FloatValue (0.3f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("bottomradius"), L"Bottom Radius", NE::ValuePtr (new NE::FloatValue (0.5f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("height"), L"Height", NE::ValuePtr (new NE::FloatValue (1.0f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("segmentation"), L"Segmentation", NE::ValuePtr (new NE::IntValue (25)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

NE::ValueConstPtr ConeNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr material = EvaluateInputSlot (NE::SlotId ("material"), env);
	NE::ValueConstPtr transformation = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	NE::ValueConstPtr topRadiusValue = EvaluateInputSlot (NE::SlotId ("topradius"), env);
	NE::ValueConstPtr bottomRadiusValue = EvaluateInputSlot (NE::SlotId ("bottomradius"), env);
	NE::ValueConstPtr heightValue = EvaluateInputSlot (NE::SlotId ("height"), env);
	NE::ValueConstPtr segmentationValue = EvaluateInputSlot (NE::SlotId ("segmentation"), env);

	if (!NE::IsComplexType<MaterialValue> (material) || !NE::IsComplexType<TransformationValue> (transformation) || !NE::IsComplexType<NE::NumberValue> (topRadiusValue) || !NE::IsComplexType<NE::NumberValue> (bottomRadiusValue) || !NE::IsComplexType<NE::NumberValue> (heightValue) || !NE::IsComplexType<NE::NumberValue> (segmentationValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {material, transformation, topRadiusValue, bottomRadiusValue, heightValue, segmentationValue}, [&] (const NE::ValueCombination& combination) {
		int segmentation = NE::NumberValue::ToInteger (combination.GetValue (5));
		Modeler::ShapePtr shape (new Modeler::ConeShape (
			MaterialValue::Get (combination.GetValue (0)),
			TransformationValue::Get (combination.GetValue (1)),
			NE::NumberValue::ToDouble (combination.GetValue (2)),
			NE::NumberValue::ToDouble (combination.GetValue (3)),
			NE::NumberValue::ToDouble (combination.GetValue (4)),
			segmentation,
			IsSmooth (segmentation)
		));
		if (!shape->Check ()) {
			return false;
		}
		result->Push (NE::ValuePtr (new ShapeValue (shape)));
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return result;
}

void ConeNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	ShapeNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<ConeNode, NE::FloatValue> (parameterList, L"Top Radius", NUIE::ParameterType::Float, NE::SlotId ("topradius"));
	NUIE::RegisterSlotDefaultValueNodeParameter<ConeNode, NE::FloatValue> (parameterList, L"Bottom Radius", NUIE::ParameterType::Float, NE::SlotId ("bottomradius"));
	NUIE::RegisterSlotDefaultValueNodeParameter<ConeNode, NE::FloatValue> (parameterList, L"Height", NUIE::ParameterType::Float, NE::SlotId ("height"));
	NUIE::RegisterSlotDefaultValueNodeParameter<ConeNode, NE::IntValue> (parameterList, L"Segmentation", NUIE::ParameterType::Integer, NE::SlotId ("segmentation"));
}

NE::Stream::Status ConeNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ConeNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

SphereNode::SphereNode () :
	SphereNode (L"", NUIE::Point ())
{

}

SphereNode::SphereNode (const std::wstring& name, const NUIE::Point& position) :
	ShapeNode (name, position)
{

}

void SphereNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("material"), L"Material", NE::ValuePtr (new MaterialValue (Modeler::DefaultMaterial)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("radius"), L"Radius", NE::ValuePtr (new NE::FloatValue (0.5f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("segmentation"), L"Segmentation", NE::ValuePtr (new NE::IntValue (25)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

NE::ValueConstPtr SphereNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr material = EvaluateInputSlot (NE::SlotId ("material"), env);
	NE::ValueConstPtr transformation = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	NE::ValueConstPtr radiusValue = EvaluateInputSlot (NE::SlotId ("radius"), env);
	NE::ValueConstPtr segmentationValue = EvaluateInputSlot (NE::SlotId ("segmentation"), env);

	if (!NE::IsComplexType<MaterialValue> (material) || !NE::IsComplexType<TransformationValue> (transformation) || !NE::IsComplexType<NE::NumberValue> (radiusValue) || !NE::IsComplexType<NE::NumberValue> (segmentationValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {material, transformation, radiusValue, segmentationValue}, [&] (const NE::ValueCombination& combination) {
		int segmentation = NE::NumberValue::ToInteger (combination.GetValue (3));
		Modeler::ShapePtr shape (new Modeler::SphereShape (
			MaterialValue::Get (combination.GetValue (0)),
			TransformationValue::Get (combination.GetValue (1)),
			NE::NumberValue::ToDouble (combination.GetValue (2)),
			segmentation,
			IsSmooth (segmentation)
		));
		if (!shape->Check ()) {
			return false;
		}
		result->Push (NE::ValuePtr (new ShapeValue (shape)));
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return result;
}

void SphereNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	ShapeNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<SphereNode, NE::FloatValue> (parameterList, L"Radius", NUIE::ParameterType::Float, NE::SlotId ("radius"));
	NUIE::RegisterSlotDefaultValueNodeParameter<SphereNode, NE::IntValue> (parameterList, L"Segmentation", NUIE::ParameterType::Integer, NE::SlotId ("segmentation"));
}

NE::Stream::Status SphereNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status SphereNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

TorusNode::TorusNode () :
	TorusNode (L"", NUIE::Point ())
{

}

TorusNode::TorusNode (const std::wstring& name, const NUIE::Point& position) :
	ShapeNode (name, position)
{

}

void TorusNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("material"), L"Material", NE::ValuePtr (new MaterialValue (Modeler::DefaultMaterial)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("outerradius"), L"Outer Radius", NE::ValuePtr (new NE::FloatValue (0.5f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("innerradius"), L"Inner Radius", NE::ValuePtr (new NE::FloatValue (0.3f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("outersegmentation"), L"Outer Segmentation", NE::ValuePtr (new NE::IntValue (25)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("innersegmentation"), L"Inner Segmentation", NE::ValuePtr (new NE::IntValue (25)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

NE::ValueConstPtr TorusNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr material = EvaluateInputSlot (NE::SlotId ("material"), env);
	NE::ValueConstPtr transformation = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	NE::ValueConstPtr outerRadiusValue = EvaluateInputSlot (NE::SlotId ("outerradius"), env);
	NE::ValueConstPtr innerRadiusValue = EvaluateInputSlot (NE::SlotId ("innerradius"), env);
	NE::ValueConstPtr outerSegmentationValue = EvaluateInputSlot (NE::SlotId ("outersegmentation"), env);
	NE::ValueConstPtr innerSegmentationValue = EvaluateInputSlot (NE::SlotId ("innersegmentation"), env);

	if (!NE::IsComplexType<MaterialValue> (material) || !NE::IsComplexType<TransformationValue> (transformation) || !NE::IsComplexType<NE::NumberValue> (outerRadiusValue) || !NE::IsComplexType<NE::NumberValue> (innerRadiusValue) || !NE::IsComplexType<NE::NumberValue> (outerSegmentationValue) || !NE::IsComplexType<NE::NumberValue> (innerSegmentationValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {material, transformation, outerRadiusValue, innerRadiusValue, outerSegmentationValue, innerSegmentationValue}, [&] (const NE::ValueCombination& combination) {
		int outerSegmentation = NE::NumberValue::ToInteger (combination.GetValue (4));
		int innerSegmentation = NE::NumberValue::ToInteger (combination.GetValue (5));
		Modeler::ShapePtr shape (new Modeler::TorusShape (
			MaterialValue::Get (combination.GetValue (0)),
			TransformationValue::Get (combination.GetValue (1)),
			NE::NumberValue::ToDouble (combination.GetValue (2)),
			NE::NumberValue::ToDouble (combination.GetValue (3)),
			outerSegmentation,
			innerSegmentation,
			IsSmooth (outerSegmentation) && IsSmooth (innerSegmentation)
		));
		if (!shape->Check ()) {
			return false;
		}
		result->Push (NE::ValuePtr (new ShapeValue (shape)));
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return result;
}

void TorusNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	ShapeNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<TorusNode, NE::FloatValue> (parameterList, L"Outer Radius", NUIE::ParameterType::Float, NE::SlotId ("outerradius"));
	NUIE::RegisterSlotDefaultValueNodeParameter<TorusNode, NE::FloatValue> (parameterList, L"Inner Radius", NUIE::ParameterType::Float, NE::SlotId ("innerradius"));
	NUIE::RegisterSlotDefaultValueNodeParameter<TorusNode, NE::IntValue> (parameterList, L"Outer Segmentation", NUIE::ParameterType::Integer, NE::SlotId ("outersegmentation"));
	NUIE::RegisterSlotDefaultValueNodeParameter<TorusNode, NE::IntValue> (parameterList, L"Inner Segmentation", NUIE::ParameterType::Integer, NE::SlotId ("innersegmentation"));
}

NE::Stream::Status TorusNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TorusNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	return outputStream.GetStatus ();
}

static void SetPlatonicNodeType (Modeler::PlatonicSolidType type, NUIE::NodeUIManager& uiManager, std::shared_ptr<PlatonicNode>& platonicNode)
{
	platonicNode->SetType (type);
	uiManager.InvalidateNodeValue (platonicNode);
	uiManager.InvalidateNodeDrawing (platonicNode);
}

PlatonicNode::PlatonicNode () :
	PlatonicNode (L"", NUIE::Point ())
{

}

PlatonicNode::PlatonicNode (const std::wstring& name, const NUIE::Point& position) :
	ShapeNode (name, position),
	type (Modeler::PlatonicSolidType::Tetrahedron)
{

}

void PlatonicNode::Initialize ()
{
	ShapeNode::Initialize ();
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("material"), L"Material", NE::ValuePtr (new MaterialValue (Modeler::DefaultMaterial)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", NE::ValuePtr (new TransformationValue (glm::dmat4 (1.0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("radius"), L"Radius", NE::ValuePtr (new NE::FloatValue (1.0f)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("shape"), L"Shape")));
}

NE::ValueConstPtr PlatonicNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr material = EvaluateInputSlot (NE::SlotId ("material"), env);
	NE::ValueConstPtr transformation = EvaluateInputSlot (NE::SlotId ("transformation"), env);
	NE::ValueConstPtr radiusValue = EvaluateInputSlot (NE::SlotId ("radius"), env);

	if (!NE::IsComplexType<MaterialValue> (material) || !NE::IsComplexType<TransformationValue> (transformation) || !NE::IsComplexType<NE::NumberValue> (radiusValue)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	bool isValid = BI::CombineValues (this, {material, transformation, radiusValue}, [&] (const NE::ValueCombination& combination) {
		Modeler::ShapePtr shape (new Modeler::PlatonicShape (
			MaterialValue::Get (combination.GetValue (0)),
			TransformationValue::Get (combination.GetValue (1)),
			type,
			NE::NumberValue::ToDouble (combination.GetValue (2))
		));
		if (!shape->Check ()) {
			return false;
		}
		result->Push (NE::ValuePtr (new ShapeValue (shape)));
		return true;
	});

	if (!isValid) {
		return nullptr;
	}
	return result;
}

void PlatonicNode::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	class SetTypeCommand : public NUIE::NodeCommand
	{
	public:
		SetTypeCommand (const std::wstring& name, bool isChecked, Modeler::PlatonicSolidType type) :
			NUIE::NodeCommand (name, isChecked),
			type (type)
		{

		}

		virtual bool IsApplicableTo (const NUIE::UINodeConstPtr& uiNode) override
		{
			return NE::Node::IsTypeConst<PlatonicNode> (uiNode);
		}

		virtual void Do (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment&, NUIE::UINodePtr& uiNode) override
		{
			std::shared_ptr<PlatonicNode> platonicNode = std::dynamic_pointer_cast<PlatonicNode> (uiNode);
			SetPlatonicNodeType (type, uiManager, platonicNode);
		}

	private:
		Modeler::PlatonicSolidType type;
	};

	ShapeNode::RegisterCommands (commandRegistrator);
	NUIE::NodeGroupCommandPtr setShapeTypeGroup (new NUIE::NodeGroupCommand<NUIE::NodeCommandPtr> (L"Set Type"));
	setShapeTypeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetTypeCommand (L"Tetrahedron", type == Modeler::PlatonicSolidType::Tetrahedron, Modeler::PlatonicSolidType::Tetrahedron)));
	setShapeTypeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetTypeCommand (L"Octahedron", type == Modeler::PlatonicSolidType::Octahedron, Modeler::PlatonicSolidType::Octahedron)));
	setShapeTypeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetTypeCommand (L"Hexahedron", type == Modeler::PlatonicSolidType::Hexahedron, Modeler::PlatonicSolidType::Hexahedron)));
	setShapeTypeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetTypeCommand (L"Dodecahedron", type == Modeler::PlatonicSolidType::Dodecahedron, Modeler::PlatonicSolidType::Dodecahedron)));
	setShapeTypeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetTypeCommand (L"Icosahedron", type == Modeler::PlatonicSolidType::Icosahedron, Modeler::PlatonicSolidType::Icosahedron)));
	commandRegistrator.RegisterNodeGroupCommand (setShapeTypeGroup);

}

void PlatonicNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class ShapeTypeParameter : public NUIE::EnumerationNodeParameter<PlatonicNode>
	{
	public:
		ShapeTypeParameter () :
			NUIE::EnumerationNodeParameter<PlatonicNode> (L"Type", { L"Tetrahedron", L"Octahedron", L"Hexahedron", L"Dodecahedron", L"Icosahedron" })
		{

		}

		virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			std::shared_ptr<const PlatonicNode> platonicNode = std::dynamic_pointer_cast<const PlatonicNode> (uiNode);
			int typeInt = (int) platonicNode->GetType ();
			return NE::ValuePtr (new NE::IntValue (typeInt));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
		{
			std::shared_ptr<PlatonicNode> platonicNode = std::dynamic_pointer_cast<PlatonicNode> (uiNode);
			int typeInt = NE::IntValue::Get (value);
			Modeler::PlatonicSolidType type = (Modeler::PlatonicSolidType) typeInt;
			SetPlatonicNodeType (type, uiManager, platonicNode);
			return true;
		}
	};

	ShapeNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<PlatonicNode, NE::FloatValue> (parameterList, L"Radius", NUIE::ParameterType::Float, NE::SlotId ("radius"));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new ShapeTypeParameter ()));
}

NE::Stream::Status PlatonicNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ShapeNode::Read (inputStream);
	int typeInt = 0;
	inputStream.Read (typeInt);
	type = (Modeler::PlatonicSolidType) typeInt;
	return inputStream.GetStatus ();
}

NE::Stream::Status PlatonicNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	ShapeNode::Write (outputStream);
	outputStream.Write ((int) type);
	return outputStream.GetStatus ();
}

Modeler::PlatonicSolidType PlatonicNode::GetType () const
{
	return type;
}

void PlatonicNode::SetType (Modeler::PlatonicSolidType newType)
{
	type = newType;
}
