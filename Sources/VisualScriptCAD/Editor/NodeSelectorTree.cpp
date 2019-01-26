#include "NodeSelectorTree.hpp"

#include "BI_InputUINodes.hpp"
#include "BI_ArithmeticUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "NodeRegistry.hpp"

class TreeItemData : public wxTreeItemData
{
public:
	TreeItemData (NodeTypeId nodeType) :
		nodeType (nodeType)
	{

	}

	NodeTypeId nodeType;
};

static std::wstring ToUpperCase (const std::wstring& text)
{
	std::wstring upperCaseText = text;
	std::transform (upperCaseText.begin (), upperCaseText.end (), upperCaseText.begin (), ::toupper);
	return upperCaseText;
}

class NodeTreeFiller
{
public:
	NodeTreeFiller (wxTreeCtrl* nodeTree, const std::wstring& searchText) :
		nodeTree (nodeTree),
		searchText (ToUpperCase (searchText))
	{
	}

	void Fill ()
	{
		nodeTree->Freeze ();
		nodeTree->DeleteAllItems ();

		const NodeRegistry& nodeRegistry = NodeRegistry::Instance ();
		wxTreeItemId rootId = nodeTree->AddRoot (L"");
		nodeRegistry.EnumerateGroups ([&] (const std::wstring& groupName) {
			AddTreeGroup (rootId, groupName, [&] (wxTreeItemId groupId) {
				nodeRegistry.EnumerateGroupNodes (groupName, [&] (const NodeRegistry::NodeData& nodeData) {
					AddTreeItem (groupId, nodeData.GetNodeName (), nodeData.GetNodeType ());
				});
			});
		});

		nodeTree->ExpandAll ();
		nodeTree->Thaw ();
	}

private:
	void AddTreeGroup (wxTreeItemId rootId, const std::wstring& groupName, const std::function<void (wxTreeItemId)>& filler)
	{
		wxTreeItemId groupId = nodeTree->AppendItem (rootId, groupName);
		filler (groupId);
		if (nodeTree->GetChildrenCount (groupId) == 0) {
			nodeTree->Delete (groupId);
		}
	}

	void AddTreeItem (wxTreeItemId parentId, const std::wstring& text, NodeTypeId nodeType)
	{
		if (NeedToShow (text)) {
			nodeTree->AppendItem (parentId, text, -1, -1, new TreeItemData (nodeType));
		}
	}

	bool NeedToShow (const std::wstring& text) const
	{
		if (searchText.empty ()) {
			return true;
		}
		std::wstring upperCaseText = ToUpperCase (text);
		return upperCaseText.find (searchText) != std::wstring::npos;
	}

	wxTreeCtrl*		nodeTree;
	std::wstring	searchText;
};

NodeSelectorTree::NodeSelectorTree (wxWindow *parent, const wxSize& size, WXAS::NodeEditorControl* nodeEditor) :
	wxPanel (parent, wxID_ANY, wxDefaultPosition, size),
	boxSizer (new wxBoxSizer (wxVERTICAL)),
	searchControl (new wxTextCtrl (this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize)),
	nodeTree (new wxTreeCtrl (this, wxID_ANY, wxDefaultPosition, wxSize (200, 400), wxTR_HIDE_ROOT | wxTR_LINES_AT_ROOT | wxTR_HAS_BUTTONS | wxTR_SINGLE)),
	nodeEditor (nodeEditor)
{
	NodeTreeFiller filler (nodeTree, L"");
	filler.Fill ();

	boxSizer->Add (searchControl, 0, wxEXPAND, 0);
	boxSizer->AddSpacer (2);
	boxSizer->Add (nodeTree, 1, wxEXPAND, 0);
	SetSizerAndFit (boxSizer);
}

void NodeSelectorTree::OnText (wxCommandEvent&)
{
	NodeTreeFiller filler (nodeTree, searchControl->GetValue ().ToStdWstring ());
	filler.Fill ();
}

void NodeSelectorTree::OnDoubleClick (wxTreeEvent& evt)
{
	wxTreeItemData* itemData = nodeTree->GetItemData (evt.GetItem ());
	if (itemData == nullptr) {
		return;
	}

	TreeItemData* treeItemData = dynamic_cast<TreeItemData*> (itemData);
	wxSize screenSize = nodeEditor->GetClientSize ();
	NUIE::Rect viewRect = NUIE::Rect::FromPositionAndSize (NUIE::Point (0.0, 0.0), NUIE::Size (screenSize.GetX (), screenSize.GetY ()));
	CreateNode (nodeEditor, viewRect.GetCenter (), treeItemData->nodeType);
}

void NodeSelectorTree::OnBeginDrag (wxTreeEvent& evt)
{
	wxTreeItemData* itemData = nodeTree->GetItemData (evt.GetItem ());
	if (itemData == nullptr) {
		return;
	}

	const NodeRegistry& nodeRegistry = NodeRegistry::Instance ();
	TreeItemData* treeItemData = dynamic_cast<TreeItemData*> (itemData);
	if (!nodeRegistry.Contains (treeItemData->nodeType)) {
		return;
	}

	wxTextDataObject textDataObject (nodeRegistry.GetNodeData (treeItemData->nodeType)->GetNodeName ());
	wxDropSource dropSource (textDataObject, nodeTree, wxStockCursor::wxCURSOR_CROSS);
	dropSource.DoDragDrop (wxDrag_CopyOnly);
}

BEGIN_EVENT_TABLE (NodeSelectorTree, wxPanel)
EVT_TEXT (wxID_ANY, NodeSelectorTree::OnText)
EVT_TREE_ITEM_ACTIVATED (wxID_ANY, NodeSelectorTree::OnDoubleClick)
EVT_TREE_BEGIN_DRAG (wxID_ANY, NodeSelectorTree::OnBeginDrag)
END_EVENT_TABLE ()

void CreateNode (WXAS::NodeEditorControl* nodeEditor, NUIE::Point viewPosition, NodeTypeId nodeType)
{
	if (!NodeRegistry::Instance ().Contains (nodeType)) {
		return;
	}
	NUIE::Point modelPosition = nodeEditor->ViewToModel (viewPosition);
	NUIE::UINodePtr uiNode = NodeRegistry::Instance ().GetNodeData (nodeType)->CreateNode (modelPosition);
	nodeEditor->AddNode (uiNode);
}

void CreateNode (WXAS::NodeEditorControl* nodeEditor, NUIE::Point viewPosition, const std::wstring& nodeName)
{
	if (!NodeRegistry::Instance ().Contains (nodeName)) {
		return;
	}
	CreateNode (nodeEditor, viewPosition, NodeRegistry::Instance ().GetNodeData (nodeName)->GetNodeType ());
}
