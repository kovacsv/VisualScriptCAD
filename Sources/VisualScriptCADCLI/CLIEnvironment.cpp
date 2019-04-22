#include "CLIEnvironment.hpp"

#include "NE_StringSettings.hpp"
#include "NUIE_SkinParams.hpp"
#include "NUIE_DrawingContext.hpp"
#include "NUIE_EventHandlers.hpp"

namespace CLI
{

NodeUIEnvironment::NodeUIEnvironment (const NE::EvaluationDataPtr& evalData) :
	NUIE::NodeUIEnvironment (),
	evaluationEnv (evalData)
{
	
}

const NE::StringSettings& NodeUIEnvironment::GetStringSettings ()
{
	return NE::GetDefaultStringSettings ();
}

const NUIE::SkinParams& NodeUIEnvironment::GetSkinParams ()
{
	return NUIE::GetDefaultSkinParams ();
}

NUIE::DrawingContext& NodeUIEnvironment::GetDrawingContext ()
{
	static NUIE::NullDrawingContext nullDrawingContext;
	return nullDrawingContext;
}
	
NE::EvaluationEnv& NodeUIEnvironment::GetEvaluationEnv ()
{
	return evaluationEnv;
}

void NodeUIEnvironment::OnEvaluationBegin ()
{

}

void NodeUIEnvironment::OnEvaluationEnd ()
{

}

void NodeUIEnvironment::OnValuesRecalculated ()
{
		
}

void NodeUIEnvironment::OnRedrawRequested ()
{

}

NUIE::EventHandlers& NodeUIEnvironment::GetEventHandlers ()
{
	static NUIE::NullEventHandlers nullEventHandlers;
	return nullEventHandlers;
}

double NodeUIEnvironment::GetMouseMoveMinOffset ()
{
	return 2.0;
}

}
