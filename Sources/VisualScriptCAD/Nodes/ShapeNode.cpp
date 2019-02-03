#include "ShapeNode.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "MaterialNode.hpp"
#include "TransformationNodes.hpp"
#include "ModelEvaluationData.hpp"

NE::SerializationInfo			ShapeNode::serializationInfo (NE::ObjectVersion (1));

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
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature ()));
}

void ShapeNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BI::BasicUINode::RegisterParameters (parameterList);
}

void ShapeNode::ProcessValue (const NE::ValueConstPtr& value, NE::EvaluationEnv& env) const
{
	std::shared_ptr<BI::EnableDisableFeature> enableDisable = GetEnableDisableFeature (this);
	if (enableDisable->GetEnableState ()) {
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
		if (enableDisable->GetEnableState ()) {
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
			Modeler::MeshId meshId = evalData->AddMesh (shape->GenerateMesh ());
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
