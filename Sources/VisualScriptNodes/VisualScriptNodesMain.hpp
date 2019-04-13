#ifndef VISUALSCRIPTNODESMAIN_HPP
#define VISUALSCRIPTNODESMAIN_HPP

#include <string>
#include <memory>

class NodeUICallbackInterface
{
public:
	NodeUICallbackInterface ();
	virtual ~NodeUICallbackInterface ();

	virtual bool EditExpression (std::wstring& expression) = 0;
};

typedef std::shared_ptr<NodeUICallbackInterface> NodeUICallbackInterfacePtr;

NodeUICallbackInterfacePtr	GetNodeUICallbackInterface ();
void						SetNodeUICallbackInterface (const NodeUICallbackInterfacePtr& newInterface);

#endif
