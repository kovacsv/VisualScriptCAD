#ifndef MODELEVALUATIONDATA_HPP
#define MODELEVALUATIONDATA_HPP

#include "NE_EvaluationEnv.hpp"
#include "Model.hpp"

class ModelEvaluationData : public NE::EvaluationData
{
public:
	ModelEvaluationData ();
	virtual ~ModelEvaluationData ();

	Modeler::Model&								GetModel ();
	Modeler::MeshId								AddMesh (const Modeler::Mesh& mesh);
	void										RemoveMesh (Modeler::MeshId meshId);

	const Modeler::Model&						GetModel () const;
	const std::unordered_set<Modeler::MeshId>&	GetAddedMeshes () const;
	const std::unordered_set<Modeler::MeshId>&	GetDeletedMeshes () const;

	void										Clear ();
	void										ClearAddedDeletedMeshes ();

private:
	Modeler::Model							model;
	std::unordered_set<Modeler::MeshId>		addedMeshes;
	std::unordered_set<Modeler::MeshId>		deletedMeshes;
};

#endif
