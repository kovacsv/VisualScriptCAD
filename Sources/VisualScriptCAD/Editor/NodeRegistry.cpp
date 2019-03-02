#include "NodeRegistry.hpp"

#include "BI_BuiltInNodes.hpp"
#include "MaterialNode.hpp"
#include "ShapeNodes.hpp"
#include "Basic3DNodes.hpp"
#include "TransformationNodes.hpp"
#include "BooleanNodes.hpp"
#include "ExpressionNode.hpp"

static const NodeRegistry nodeRegistry;

NodeRegistry::NodeData::NodeData (NodeTypeId type, const std::wstring& name, const CreatorFunction& creator) :
	type (type),
	name (name),
	creator (creator)
{
}

NodeTypeId NodeRegistry::NodeData::GetNodeType () const
{
	return type;
}

const std::wstring& NodeRegistry::NodeData::GetNodeName  () const
{
	return name;
}

NUIE::UINodePtr NodeRegistry::NodeData::CreateNode (const NUIE::Point& position) const
{
	return creator (position);
}

NodeRegistry::NodeRegistry () :
	nextNodeTypeId (0)
{
	RegisterNode (L"Input Nodes", L"Boolean",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::BooleanNode (L"Boolean", position, true));}
	);
	RegisterNode (L"Input Nodes", L"Integer",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::IntegerUpDownNode (L"Integer", position, 0, 1));}
	);
	RegisterNode (L"Input Nodes", L"Number",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", position, 0.0, 1.0)); }
	);
	RegisterNode (L"Input Nodes", L"Integer Increment",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::IntegerIncrementedNode (L"Int Icrement", position)); }
	);
	RegisterNode (L"Input Nodes", L"Number Increment",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::DoubleIncrementedNode (L"Num Increment", position)); }
	);
	RegisterNode (L"Input Nodes", L"Number Distribute",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::DoubleDistributedNode (L"Num Distribute", position)); }
	);
	RegisterNode (L"Arithmetic Nodes", L"Addition",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::AdditionNode (L"Addition", position)); }
	);
	RegisterNode (L"Arithmetic Nodes", L"Subtraction",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::SubtractionNode (L"Subtraction", position)); }
	);
	RegisterNode (L"Arithmetic Nodes", L"Multiplication",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::MultiplicationNode (L"Multiplication", position)); }
	);
	RegisterNode (L"Arithmetic Nodes", L"Division",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::DivisionNode (L"Division", position)); }
	);
	RegisterNode (L"Arithmetic Nodes", L"Expression",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new ExpressionNode (L"Expression", position)); }
	);
	RegisterNode (L"Point Nodes", L"Point 2D",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new Point2DNode (L"Point 2D", position)); }
	);
	RegisterNode (L"Point Nodes", L"Point",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new PointNode (L"Point", position)); }
	);
	RegisterNode (L"Point Nodes", L"Vector",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new VectorNode (L"Vector", position)); }
	);
	RegisterNode (L"Point Nodes", L"Line Points",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new LinePointsNode (L"Line Points", position)); }
	);
	RegisterNode (L"Point Nodes", L"Arc Points",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new ArcPointsNode (L"Arc Points", position)); }
	);
	RegisterNode (L"Shape Nodes", L"Material",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new MaterialNode (L"Material", position)); }
	);
	RegisterNode (L"Shape Nodes", L"Box",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BoxNode (L"Box", position)); }
	);
	RegisterNode (L"Shape Nodes", L"Cylinder",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new CylinderNode (L"Cylinder", position)); }
	);
	RegisterNode (L"Shape Nodes", L"Cone",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new ConeNode (L"Cone", position)); }
	);
	RegisterNode (L"Shape Nodes", L"Sphere",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new SphereNode (L"Sphere", position)); }
	);
	RegisterNode (L"Shape Nodes", L"Torus",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new TorusNode (L"Torus", position)); }
	);
	RegisterNode (L"Shape Nodes", L"Prism",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new PrismNode (L"Prism", position)); }
	);
	RegisterNode (L"Shape Nodes", L"Platonic",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new PlatonicNode (L"Platonic", position)); }
	);
	RegisterNode (L"Matrix Nodes", L"Translation Matrix",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new TranslationMatrixNode (L"Translation Matrix", position)); }
	);
	RegisterNode (L"Matrix Nodes", L"Translation From Point",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new PointTranslationNode (L"Translation From Point", position)); }
	);
	RegisterNode (L"Matrix Nodes", L"Rotation Matrix",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new RotationMatrixNode (L"Rotation Matrix", position)); }
	);
	RegisterNode (L"Matrix Nodes", L"Scale Matrix",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new ScaleMatrixNode (L"Scale Matrix", position)); }
	);
	RegisterNode (L"Matrix Nodes", L"Matrix Combination",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new MatrixCombinationNode (L"Matrix Combination", position)); }
	);
	RegisterNode (L"Transformation Nodes", L"Transform Point",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new TransformPointNode (L"Transform Point", position)); }
	);
	RegisterNode (L"Transformation Nodes", L"Transform Shape",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new TransformShapeNode (L"Transform Shape", position)); }
	);
	RegisterNode (L"Boolean Nodes", L"Difference",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BooleanNode (L"Difference", position, CGALOperations::BooleanOperation::Difference)); }
	);
	RegisterNode (L"Boolean Nodes", L"Intersection",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BooleanNode (L"Intersection", position, CGALOperations::BooleanOperation::Intersection)); }
	);
	RegisterNode (L"Boolean Nodes", L"Union",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BooleanNode (L"Union", position, CGALOperations::BooleanOperation::Union)); }
	);
	RegisterNode (L"Other Nodes", L"Viewer",
		[] (const NUIE::Point& position) { return NUIE::UINodePtr (new BI::MultiLineViewerNode (L"Viewer", position, 5)); }
	);
}

bool NodeRegistry::Contains (size_t type) const
{
	return GetNodeData (type) != nullptr;
}

bool NodeRegistry::Contains (const std::wstring& name) const
{
	return GetNodeData (name) != nullptr;
}

const NodeRegistry::NodeData* NodeRegistry::GetNodeData (size_t type) const
{
	auto found = typeToNodeData.find (type);
	if (found == typeToNodeData.end ()) {
		return nullptr;
	}
	return &nodeRegistry.at (found->second);
}

const NodeRegistry::NodeData* NodeRegistry::GetNodeData (const std::wstring& name) const
{
	auto found = nameToNodeData.find (name);
	if (found == nameToNodeData.end ()) {
		return nullptr;
	}
	return &nodeRegistry.at (found->second);
}

void NodeRegistry::EnumerateGroups (const std::function<void (const std::wstring&)>& processor) const
{
	for (const GroupData& group : groupRegistry) {
		processor (group.name);
	}
}

void NodeRegistry::EnumerateGroupNodes (const std::wstring& groupName, const std::function<void (const NodeData&)>& processor) const
{
	auto found = nameToGroupData.find (groupName);
	if (found == nameToGroupData.end ()) {
		return;
	}
	const GroupData& groupData = groupRegistry[found->second];
	for (NodeTypeId nodeType : groupData.nodes) {
		processor (nodeRegistry[nodeType]);
	}
}

const NodeRegistry& NodeRegistry::Instance ()
{
	return ::nodeRegistry;
}

void NodeRegistry::RegisterNode (const std::wstring& group, const std::wstring& name, const CreatorFunction& creator)
{
	auto foundGroup = nameToGroupData.find (group);
	if (foundGroup == nameToGroupData.end ()) {
		GroupData groupData = { group, std::vector<NodeTypeId> {} };
		nameToGroupData.insert ({ group, groupRegistry.size () });
		groupRegistry.push_back (groupData);
	}

	NodeTypeId type = nextNodeTypeId++;
	NodeData nodeData = {
		type,
		name,
		creator
	};

	typeToNodeData.insert ({ type, nodeRegistry.size () });
	nameToNodeData.insert ({ name, nodeRegistry.size () });
	groupRegistry[nameToGroupData[group]].nodes.push_back (nodeRegistry.size ());
	nodeRegistry.push_back (nodeData);
}
