#include "NodeEditorControl.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Debug.hpp"
#include "WXAS_ParameterDialog.hpp"

#include "ModelEvaluationData.hpp"
#include "NodeRegistry.hpp"

class NodeEditorUIEnvironment : public WXAS::NodeEditorUIEnvironment
{
public:
	NodeEditorUIEnvironment (	WXAS::NodeEditorControl* nodeEditorControl,
								std::shared_ptr<NE::StringSettings> stringSettings,
								std::shared_ptr<NUIE::SkinParams> skinParams,
								std::shared_ptr<NUIE::EventHandlers> eventHandlers,
								NE::EvaluationEnv& evaluationEnv,
								ModelSynchronizer& synchronizer) :
		WXAS::NodeEditorUIEnvironment (nodeEditorControl, stringSettings, skinParams, eventHandlers, evaluationEnv),
		synchronizer (synchronizer)
	{
	}

	virtual void OnValuesRecalculated () override
	{
		synchronizer.Synchronize ();
	}

private:
	ModelSynchronizer& synchronizer;
};

NodeEditorDropTarget::NodeEditorDropTarget (WXAS::NodeEditorControl* nodeEditorControl) :
	wxTextDropTarget (),
	nodeEditorControl (nodeEditorControl)
{

}

bool NodeEditorDropTarget::OnDropText (wxCoord x, wxCoord y, const wxString& data)
{
	CreateNode (nodeEditorControl, NUIE::Point (x, y), data.ToStdWstring ());
	return true;
}

NodeEditorControl::NodeEditorControl (wxWindow *parent, const std::shared_ptr<NE::EvaluationData>& evalData, ModelSynchronizer& synchronizer) :
	wxPanel (parent),
	nodeEditorControl (new WXAS::NodeEditorControl (this)),
	nodeSelectorTree (new NodeSelectorTree (this, wxSize (200, 0), nodeEditorControl)),
	boxSizer (new wxBoxSizer (wxHORIZONTAL)),
	evalEnv (evalData),
	synchronizer (synchronizer)
{
	std::shared_ptr<WXAS::NodeEditorUIEnvironment> uiEnvironment = std::shared_ptr<WXAS::NodeEditorUIEnvironment> (
		new NodeEditorUIEnvironment (
			nodeEditorControl,
			NE::StringSettingsPtr (new NE::BasicStringSettings (NE::GetDefaultStringSettings ())),
			NUIE::SkinParamsPtr (new NUIE::BasicSkinParams (NUIE::GetDefaultSkinParams ())),
			NUIE::EventHandlersPtr (new WXAS::NodeEditorEventHandlers (nodeEditorControl)),
			evalEnv, synchronizer
		)
	);

	nodeEditorControl->Init (uiEnvironment);

	NodeEditorDropTarget* dropTarget = new NodeEditorDropTarget (nodeEditorControl);
	nodeEditorControl->SetDropTarget (dropTarget);

	boxSizer->Add (nodeSelectorTree, 0, wxEXPAND, 0);
	boxSizer->Add (nodeEditorControl, 1, wxEXPAND, 0);
	SetSizerAndFit (boxSizer);
}

WXAS::NodeEditorControl* NodeEditorControl::GetEditor ()
{
	return nodeEditorControl;
}

void NodeEditorControl::Clear ()
{
	nodeEditorControl->New ();
}

void NodeEditorControl::OnKeyDown (wxKeyEvent& evt)
{
	nodeEditorControl->OnKeyDown (evt);
}
