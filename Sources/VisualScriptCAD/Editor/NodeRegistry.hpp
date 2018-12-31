#ifndef NODEREGISTRY_HPP
#define NODEREGISTRY_HPP

#include <functional>
#include "NUIE_UINode.hpp"

using NodeTypeId = size_t;

class NodeRegistry
{
public:
	using CreatorFunction = std::function<NUIE::UINodePtr (const NUIE::Point& position)>;

	class NodeData
	{
	public:
		NodeData (NodeTypeId type, const std::wstring& name, const CreatorFunction& creator);

		NodeTypeId				GetNodeType () const;
		const std::wstring&		GetNodeName  () const;
		NUIE::UINodePtr			CreateNode (const NUIE::Point& position) const;

	private:
		size_t				type;
		std::wstring		name;
		CreatorFunction		creator;
	};

	NodeRegistry ();

	bool						Contains (size_t type) const;
	bool						Contains (const std::wstring& name) const;

	const NodeData*				GetNodeData (size_t type) const;
	const NodeData*				GetNodeData (const std::wstring& name) const;

	void						EnumerateGroups (const std::function<void (const std::wstring&)>& processor) const;
	void						EnumerateGroupNodes (const std::wstring& groupName, const std::function<void (const NodeData&)>& processor) const;

	static const NodeRegistry&	Instance ();

private:
	struct GroupData
	{
		std::wstring name;
		std::vector<NodeTypeId> nodes;
	};

	void RegisterNode (const std::wstring& group, const std::wstring& name, const CreatorFunction& creator);

	std::vector<GroupData>						groupRegistry;
	std::vector<NodeData>						nodeRegistry;
	std::unordered_map<NodeTypeId, size_t>		typeToNodeData;
	std::unordered_map<std::wstring, size_t>	nameToNodeData;
	std::unordered_map<std::wstring, size_t>	nameToGroupData;
	NodeTypeId									nextNodeTypeId;
};

#endif
