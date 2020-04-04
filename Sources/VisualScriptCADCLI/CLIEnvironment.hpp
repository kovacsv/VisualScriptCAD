#ifndef CLIENVIRONMENT_HPP
#define CLIENVIRONMENT_HPP

#include "NE_EvaluationEnv.hpp"
#include "NUIE_NodeUIEnvironment.hpp"

namespace CLI
{

class NodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	NodeUIEnvironment (const NE::EvaluationDataPtr& evalData);

	virtual const NE::StringSettings&	GetStringSettings () override;
	virtual const NUIE::SkinParams&		GetSkinParams () override;
	virtual NUIE::DrawingContext&		GetDrawingContext () override;
	virtual double						GetWindowScale () override;

	virtual NE::EvaluationEnv&			GetEvaluationEnv () override;
	virtual void						OnEvaluationBegin () override;
	virtual void						OnEvaluationEnd () override;
	virtual void						OnValuesRecalculated () override;
	
	virtual void						OnRedrawRequested () override;
	virtual NUIE::EventHandlers&		GetEventHandlers () override;
	virtual double						GetMouseMoveMinOffset () override;

private:
	NE::EvaluationEnv					evaluationEnv;
};

}

#endif
