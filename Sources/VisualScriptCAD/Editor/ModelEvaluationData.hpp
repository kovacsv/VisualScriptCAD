#ifndef MODELEVALUATIONDATA_HPP
#define MODELEVALUATIONDATA_HPP

#include "NE_EvaluationEnv.hpp"
#include "Model.hpp"

class ModelEvaluationData : public NE::EvaluationData
{
public:
	virtual Modeler::Model&		GetModel () = 0;
	virtual Modeler::MeshId		AddMesh (const Modeler::Mesh& mesh) = 0;
	virtual void				RemoveMesh (Modeler::MeshId meshId) = 0;
};

#endif
