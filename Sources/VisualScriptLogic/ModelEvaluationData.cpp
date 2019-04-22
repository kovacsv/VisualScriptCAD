#include "ModelEvaluationData.hpp"

ModelEvaluationData::ModelEvaluationData () :
	model (),
	addedMeshes (),
	deletedMeshes ()
{
}

ModelEvaluationData::~ModelEvaluationData ()
{
}

const Modeler::Model& ModelEvaluationData::GetModel () const
{
	return model;
}

Modeler::MeshId ModelEvaluationData::AddMesh (const Modeler::Mesh& mesh)
{
	Modeler::MeshId meshId = model.AddMesh (mesh);
	addedMeshes.insert (meshId);
	return meshId;
}

void ModelEvaluationData::RemoveMesh (Modeler::MeshId meshId)
{
	model.RemoveMesh (meshId);
	deletedMeshes.insert (meshId);
}

const std::unordered_set<Modeler::MeshId>& ModelEvaluationData::GetAddedMeshes () const
{
	return addedMeshes;
}

const std::unordered_set<Modeler::MeshId>& ModelEvaluationData::GetDeletedMeshes () const
{
	return deletedMeshes;
}

void ModelEvaluationData::Clear ()
{
	model.Clear ();
	ClearAddedDeletedMeshes ();
}

void ModelEvaluationData::ClearAddedDeletedMeshes ()
{
	addedMeshes.clear ();
	deletedMeshes.clear ();
}
