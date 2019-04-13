#ifndef NODESELECTORTREE_HPP
#define NODESELECTORTREE_HPP

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/dnd.h>

#include "WXAS_NodeEditorControl.hpp"
#include "NodeRegistry.hpp"

class NodeSelectorTree : public wxPanel
{
public:
	NodeSelectorTree (wxWindow *parent, const wxSize& size, WXAS::NodeEditorControl* nodeEditor);

	void	OnText (wxCommandEvent& evt);
	void	OnDoubleClick (wxTreeEvent& evt);
	void	OnBeginDrag (wxTreeEvent& evt);

private:
	void	RegisterNodes ();

	wxBoxSizer*					boxSizer;
	wxTextCtrl*					searchControl;
	wxTreeCtrl*					nodeTree;
	WXAS::NodeEditorControl*	nodeEditor;

	DECLARE_EVENT_TABLE ()
};

void CreateNode (WXAS::NodeEditorControl* nodeEditor, NUIE::Point viewPosition, NodeRegistryId nodeType);
void CreateNode (WXAS::NodeEditorControl* nodeEditor, NUIE::Point viewPosition, const std::wstring& nodeName);

#endif
