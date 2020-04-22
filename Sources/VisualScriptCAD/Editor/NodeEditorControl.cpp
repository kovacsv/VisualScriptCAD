#include "NodeEditorControl.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Debug.hpp"
#include "WXAS_ParameterDialog.hpp"

#include "ModelEvaluationData.hpp"

const NUIE::BasicSkinParams& GetSkinParams ()
{
	static const NUIE::BasicSkinParams defaultSkinParams (
		/*backgroundColor*/ NUIE::Color (255, 255, 255),
		/*connectionLinePen*/ NUIE::Pen (NUIE::Color (0, 0, 0), 1.0),
		/*nodePadding*/ 5.0,
		/*nodeBorderPen*/ NUIE::Pen (NUIE::Color (0, 0, 0), 1.0),
		/*nodeHeaderTextFont*/ NUIE::Font (L"Arial", 16.0),
		/*nodeHeaderTextColor*/ NUIE::Color (255, 255, 255),
		/*nodeHeaderErrorTextColor*/ NUIE::Color (255, 255, 255),
		/*nodeHeaderBackgroundColor*/ NUIE::Color (120, 120, 120),
		/*nodeHeaderErrorBackgroundColor*/ NUIE::Color (225, 0, 0),
		/*nodeContentTextFont*/ NUIE::Font (L"Arial", 16.0),
		/*nodeContentTextColor*/ NUIE::Color (0, 0, 0),
		/*nodeContentBackgroundColor*/ NUIE::Color (225, 225, 225),
		/*slotTextColor*/ NUIE::Color (0, 0, 0),
		/*slotTextBackgroundColor*/ NUIE::Color (240, 240, 240),
		/*needToDrawSlotCircles*/ false,
		/*slotCircleSize*/ NUIE::Size (8.0, 8.0),
		/*selectionBlendColor*/ NUIE::BlendColor (NUIE::Color (240, 240, 240), 0.5),
		/*disabledBlendColor*/ NUIE::BlendColor (NUIE::Color (0, 138, 184), 0.2),
		/*selectionRectPen*/ NUIE::Pen (NUIE::Color (0, 138, 184), 1.0),
		/*nodeSelectionRectPen*/ NUIE::Pen (NUIE::Color (0, 138, 184), 5.0),
		/*buttonBorderPen*/ NUIE::Pen (NUIE::Color (50, 75, 100), 1.0),
		/*buttonBackgroundColor*/ NUIE::Color (150, 175, 200),
		/*textPanelTextColor*/ NUIE::Color (0, 0, 0),
		/*textPanelBackgroundColor*/ NUIE::Color (255, 255, 100),
		/*groupNameFont*/ NUIE::Font (L"Arial", 18.0),
		/*groupNameColor*/ NUIE::Color (0, 0, 0),
		/*groupBackgroundColors*/ NUIE::NamedColorSet ({
			{ L"Blue", NUIE::Color (160, 200, 240) },
			{ L"Green", NUIE::Color (160, 239, 160) },
			{ L"Red", NUIE::Color (239, 189, 160) }
		}),
		/*groupPadding*/ 10.0
	);
	return defaultSkinParams;
}

ModelUpdater::~ModelUpdater ()
{

}

class NodeEditorUIEnvironment : public WXAS::NodeEditorUIEnvironment
{
public:
	NodeEditorUIEnvironment (	WXAS::NodeEditorControl* nodeEditorControl,
								std::shared_ptr<NE::StringConverter> stringConverter,
								std::shared_ptr<NUIE::SkinParams> skinParams,
								std::shared_ptr<NUIE::EventHandlers> eventHandlers,
								NE::EvaluationEnv& evaluationEnv,
								ModelUpdater& modelUpdater) :
		WXAS::NodeEditorUIEnvironment (nodeEditorControl, stringConverter, skinParams, eventHandlers, evaluationEnv),
		modelUpdater (modelUpdater)
	{
	}

	virtual void OnValuesRecalculated () override
	{
		modelUpdater.UpdateModel ();
	}

private:
	ModelUpdater& modelUpdater;
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

NodeEditorControl::NodeEditorControl (wxWindow *parent, const std::shared_ptr<NE::EvaluationData>& evalData, ModelUpdater& modelUpdater) :
	wxPanel (parent),
	nodeEditorControl (new WXAS::NodeEditorControl (this)),
	nodeSelectorTree (new NodeSelectorTree (this, wxSize (200, 0), nodeEditorControl)),
	boxSizer (new wxBoxSizer (wxHORIZONTAL)),
	evalEnv (evalData)
{
	std::shared_ptr<WXAS::NodeEditorUIEnvironment> uiEnvironment = std::shared_ptr<WXAS::NodeEditorUIEnvironment> (
		new NodeEditorUIEnvironment (
			nodeEditorControl,
			NE::StringConverterPtr (new NE::BasicStringConverter (NE::GetDefaultStringSettings ())),
			NUIE::SkinParamsPtr (new NUIE::BasicSkinParams (GetSkinParams ())),
			NUIE::EventHandlersPtr (new WXAS::NodeEditorEventHandlers (nodeEditorControl)),
			evalEnv, modelUpdater
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
