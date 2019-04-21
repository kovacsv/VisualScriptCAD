#include "VisualScriptLogicMain.hpp"

static NodeUICallbackInterfacePtr nodeUICallbackInterface = nullptr;

NodeUICallbackInterface::NodeUICallbackInterface ()
{
}

NodeUICallbackInterface::~NodeUICallbackInterface ()
{
}

NodeUICallbackInterfacePtr GetNodeUICallbackInterface ()
{
	if (nodeUICallbackInterface == nullptr) {
		throw std::logic_error ("invalid ui callback interface");
	}
	return nodeUICallbackInterface;
}

void SetNodeUICallbackInterface (const NodeUICallbackInterfacePtr& newInterface)
{
	nodeUICallbackInterface = newInterface;
}
