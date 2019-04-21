#ifndef VISUALSCRIPTLOGICMAIN_HPP
#define VISUALSCRIPTLOGICMAIN_HPP

#include "IncludeGLM.hpp"
#include <string>
#include <vector>
#include <memory>

class NodeUICallbackInterface
{
public:
	NodeUICallbackInterface ();
	virtual ~NodeUICallbackInterface ();

	virtual bool EditExpression (std::wstring& expression) = 0;
	virtual bool EditPolygon (std::vector<glm::dvec2>& polygon) = 0;
};

typedef std::shared_ptr<NodeUICallbackInterface> NodeUICallbackInterfacePtr;

NodeUICallbackInterfacePtr	GetNodeUICallbackInterface ();
void						SetNodeUICallbackInterface (const NodeUICallbackInterfacePtr& newInterface);

#endif
