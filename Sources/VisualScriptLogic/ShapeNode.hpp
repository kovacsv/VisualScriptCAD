#ifndef SHAPENODE_HPP
#define SHAPENODE_HPP

#include "NE_GenericValue.hpp"
#include "BI_BasicUINode.hpp"
#include "Model.hpp"
#include "Shape.hpp"

class ShapeValue : public NE::GenericValue<Modeler::ShapePtr>
{
	DYNAMIC_SERIALIZABLE (ShapeValue);

public:
	ShapeValue ();
	ShapeValue (const Modeler::ShapePtr& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString (const NE::StringSettings& stringSettings) const override;
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class ShapeNode : public BI::BasicUINode
{
	SERIALIZABLE;

public:
	ShapeNode ();
	ShapeNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;
	virtual void				ProcessCalculatedValue (const NE::ValueConstPtr& value, NE::EvaluationEnv& env) const override;
	virtual void				OnFeatureChange (const NUIE::FeatureId& featureId, NE::EvaluationEnv& env) const override;
	virtual void				OnDelete (NE::EvaluationEnv& env) const override;

	void						OnCalculated (const NE::ValueConstPtr& value, NE::EvaluationEnv& env) const;
	void						OnEnabled (NE::EvaluationEnv& env) const;
	void						OnDisabled (NE::EvaluationEnv& env) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

protected:
	virtual void				DrawInplace (NUIE::NodeUIDrawingEnvironment& env) const override;

private:
	void						AddItem (NE::EvaluationEnv& env) const;
	void						RemoveItem (NE::EvaluationEnv& env) const;

	mutable std::unordered_set<Modeler::MeshId> meshes;
};

#endif
