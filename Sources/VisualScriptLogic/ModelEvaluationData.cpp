#include "ModelEvaluationData.hpp"

NodeIdUserData::NodeIdUserData (const NE::NodeId& nodeId) :
	nodeId (nodeId)
{
}

const NE::NodeId& NodeIdUserData::GetNodeId () const
{
	return nodeId;
}

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

Modeler::MeshId ModelEvaluationData::AddMesh (const Modeler::Mesh& mesh, const NE::NodeId& nodeId)
{
	Modeler::MeshId meshId = model.AddMesh (mesh);
	model.SetMeshUserData (meshId, "nodeid", Modeler::UserDataConstPtr (new NodeIdUserData (nodeId)));
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

void ModelEvaluationData::ClearAddedDeletedMeshes ()
{
	addedMeshes.clear ();
	deletedMeshes.clear ();
}

void ModelEvaluationData::Clear ()
{
	model.Clear ();
	ClearAddedDeletedMeshes ();
}
