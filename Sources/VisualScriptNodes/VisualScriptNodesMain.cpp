#include "VisualScriptNodesMain.hpp"

static NodeUICallbackInterfacePtr nodeUICallbackInterface = nullptr;

NodeUICallbackInterface::NodeUICallbackInterface ()
{
}

NodeUICallbackInterface::~NodeUICallbackInterface ()
{
}

NodeUICallbackInterfacePtr GetNodeUICallbackInterface ()
{
	return nodeUICallbackInterface;
}

void SetNodeUICallbackInterface (const NodeUICallbackInterfacePtr& newInterface)
{
	nodeUICallbackInterface = newInterface;
}
