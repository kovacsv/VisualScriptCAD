#include "CLIEnvironment.hpp"

#include "NE_StringConverter.hpp"
#include "NUIE_SkinParams.hpp"
#include "NUIE_DrawingContext.hpp"
#include "NUIE_EventHandler.hpp"
#include "NUIE_ClipboardHandler.hpp"

namespace CLI
{

NodeUIEnvironment::NodeUIEnvironment (const NE::EvaluationDataPtr& evalData) :
	NUIE::NodeUIEnvironment (),
	evaluationEnv (evalData)
{
	
}

const NE::StringConverter& NodeUIEnvironment::GetStringConverter ()
{
	return NE::GetDefaultStringConverter ();
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

double NodeUIEnvironment::GetWindowScale ()
{
	return 1.0;
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

NUIE::EventHandler& NodeUIEnvironment::GetEventHandler ()
{
	static NUIE::NullEventHandler nullEventHandler;
	return nullEventHandler;
}

NUIE::ClipboardHandler& NodeUIEnvironment::GetClipboardHandler ()
{
	static NUIE::NullClipboardHandler nullClipboardHandler;
	return nullClipboardHandler;
}

}
