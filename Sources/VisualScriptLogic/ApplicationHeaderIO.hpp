#ifndef APPLICATIONHEADERIO_HPP
#define APPLICATIONHEADERIO_HPP

#include "NUIE_NodeEditor.hpp"

class ApplicationHeaderIO : public NUIE::ExternalHeaderIO
{
public:
	ApplicationHeaderIO ();
	virtual bool Read (NE::InputStream& inputStream) const override;
	virtual void Write (NE::OutputStream& outputStream) const override;
};

extern const ApplicationHeaderIO appHeaderIO;

#endif
