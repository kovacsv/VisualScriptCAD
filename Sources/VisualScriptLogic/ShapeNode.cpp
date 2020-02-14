#include "ShapeNode.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "MaterialNode.hpp"
#include "TransformationNodes.hpp"
#include "ModelEvaluationData.hpp"

NE::DynamicSerializationInfo	ShapeValue::serializationInfo (NE::ObjectId ("{3C6EA711-831F-4A16-AC74-6A43A1AB7ACD}"), NE::ObjectVersion (1), ShapeValue::CreateSerializableInstance);
NE::SerializationInfo			ShapeNode::serializationInfo (NE::ObjectVersion (1));


ShapeValue::ShapeValue () :
	ShapeValue (nullptr)
{

}

ShapeValue::ShapeValue (const Modeler::ShapePtr& val) :
	NE::GenericValue<Modeler::ShapePtr> (val)
{

}

NE::ValuePtr ShapeValue::Clone () const
{
	return NE::ValuePtr (new ShapeValue (val->Clone ()));
}

std::wstring ShapeValue::ToString (const NE::StringSettings&) const
{
	return val->ToString ();
}

NE::Stream::Status ShapeValue::Read (NE::InputStream& inputStream)
{
	DBGBREAK ();
	NE::ObjectHeader header (inputStream);
	NE::GenericValue<Modeler::ShapePtr>::Read (inputStream);
	//val.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ShapeValue::Write (NE::OutputStream& outputStream) const
{
	DBGBREAK ();
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::GenericValue<Modeler::ShapePtr>::Write (outputStream);
	//val.Write (outputStream);
	return outputStream.GetStatus ();
}

ShapeNode::ShapeNode () :
	ShapeNode (L"", NUIE::Point ())
{

}

ShapeNode::ShapeNode (const std::wstring& name, const NUIE::Point& position) :
	BI::BasicUINode (name, position)
{

}

void ShapeNode::Initialize ()
{
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::EnableDisableFeature ()));
}

void ShapeNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BI::BasicUINode::RegisterParameters (parameterList);
}

void ShapeNode::ProcessCalculatedValue (const NE::ValueConstPtr& value, NE::EvaluationEnv& env) const
{
	std::shared_ptr<BI::EnableDisableFeature> enableDisable = GetEnableDisableFeature (this);
	if (enableDisable->GetEnableState () == BI::EnableDisableFeature::State::Enabled) {
		OnCalculated (value, env);
	}
}

void ShapeNode::OnCalculated (const NE::ValueConstPtr&, NE::EvaluationEnv& env) const
{
	RemoveItem (env);
	AddItem (env);
}

void ShapeNode::OnEnabled (NE::EvaluationEnv& env) const
{
	RemoveItem (env);
	AddItem (env);
}

void ShapeNode::OnDisabled (NE::EvaluationEnv& env) const
{
	RemoveItem (env);
}

void ShapeNode::OnFeatureChange (const NUIE::FeatureId& featureId, NE::EvaluationEnv& env) const
{
	if (featureId == BI::EnableDisableFeatureId) {
		std::shared_ptr<BI::EnableDisableFeature> enableDisable = GetEnableDisableFeature (this);
		if (enableDisable->GetEnableState () == BI::EnableDisableFeature::State::Enabled) {
			OnEnabled (env);
		} else {
			OnDisabled (env);
		}
	}
}

void ShapeNode::OnDelete (NE::EvaluationEnv& env) const
{
	RemoveItem (env);
}

void ShapeNode::AddItem (NE::EvaluationEnv& env) const
{
	if (!env.IsDataType<ModelEvaluationData> ()) {
		throw std::logic_error ("invalid evaluation data");
	}

	if (!HasCalculatedValue ()) {
		return;
	}

	NE::ValueConstPtr value = GetCalculatedValue ();
	std::shared_ptr<ModelEvaluationData> evalData = env.GetData<ModelEvaluationData> ();
	NE::FlatEnumerate (value, [&] (const NE::ValueConstPtr& innerValue) {
		const ShapeValue* shapeValue = NE::Value::Cast<ShapeValue> (innerValue.get ());
		if (shapeValue != nullptr && shapeValue->GetValue () != nullptr) {
			Modeler::ShapePtr shape = shapeValue->GetValue ();
			Modeler::MeshId meshId = evalData->AddMesh (shape->GenerateMesh (), GetId ());
			meshes.insert (meshId);
		}
	});
}

void ShapeNode::RemoveItem (NE::EvaluationEnv& env) const
{
	if (!env.IsDataType<ModelEvaluationData> ()) {
		throw std::logic_error ("invalid evaluation data");
	}

	std::shared_ptr<ModelEvaluationData> evalData = env.GetData<ModelEvaluationData> ();
	for (const auto& meshId : meshes) {
		evalData->RemoveMesh (meshId);
	}

	meshes.clear ();
}

NE::Stream::Status ShapeNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BI::BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ShapeNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BI::BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

void ShapeNode::DrawInplace (NUIE::NodeUIDrawingEnvironment& env) const
{
	std::shared_ptr<BI::EnableDisableFeature> enableDisable = GetEnableDisableFeature (this);
	enableDisable->DrawInplace (env, [&] (NUIE::NodeUIDrawingEnvironment& newEnv) {
		BI::BasicUINode::DrawInplace (newEnv);
	});
}
