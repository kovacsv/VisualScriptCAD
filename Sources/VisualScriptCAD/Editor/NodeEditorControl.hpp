#ifndef NODEEDITOR_HPP
#define NODEEDITOR_HPP

#include "WXAS_NodeEditorControl.hpp"
#include "NodeSelectorTree.hpp"
#include "Model.hpp"

class ModelUpdater
{
public:
	virtual ~ModelUpdater ();

	virtual void UpdateModel () = 0;
};

class NodeEditorDropTarget : public wxTextDropTarget
{
public:
	NodeEditorDropTarget (WXAS::NodeEditorControl* nodeEditorControl);

	virtual bool OnDropText (wxCoord x, wxCoord y, const wxString& data) override;

private:
	WXAS::NodeEditorControl* nodeEditorControl;
};

class NodeEditorControl : public wxPanel
{
public:
	NodeEditorControl (wxWindow *parent, const std::shared_ptr<NE::EvaluationData>& evalData, ModelUpdater& modelUpdater);

	WXAS::NodeEditorControl*	GetEditor ();
	void						Clear ();

	void						OnKeyDown (wxKeyEvent& evt);

private:
	WXAS::NodeEditorControl*	nodeEditorControl;
	NodeSelectorTree*			nodeSelectorTree;
	wxBoxSizer*					boxSizer;

	NE::EvaluationEnv			evalEnv;
};

#endif
