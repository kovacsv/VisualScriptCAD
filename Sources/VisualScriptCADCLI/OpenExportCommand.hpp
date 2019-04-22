#ifndef OPENEXPORTCOMMAND_HPP
#define OPENEXPORTCOMMAND_HPP

#include "CLICommand.hpp"

class OpenExportCommand : public CLI::Command
{
public:
	OpenExportCommand ();

	virtual bool Do () const override;
};

#endif
