#include <iostream>

#include "SimpleTest.hpp"

int main (int, char* argv[])
{
    std::string executablePath (argv[0]);
	SimpleTest::SetAppLocation (executablePath);
	if (!SimpleTest::RunTests ()) {
		return 1;
	}

	return 0;
}
