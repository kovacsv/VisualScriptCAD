#include "ApplicationHeaderIO.hpp"
#include "Version.hpp"
#include "VersionInfo.hpp"

ApplicationHeaderIO::ApplicationHeaderIO ()
{

}

bool ApplicationHeaderIO::Read (NE::InputStream& inputStream) const
{
	std::wstring appName;
	inputStream.Read (appName);
	if (appName != VSCAD_APP_NAME) {
		return false;
	}
	NUIE::Version readVersion;
	readVersion.Read (inputStream);
	if (readVersion > AppVersion) {
		return false;
	}
	int readFileVersion = 0;
	inputStream.Read (readFileVersion);
	if (readFileVersion != FileVersion) {
		return false;
	}
	return true;
}

void ApplicationHeaderIO::Write (NE::OutputStream& outputStream) const
{
	outputStream.Write (std::wstring (VSCAD_APP_NAME));
	AppVersion.Write (outputStream);
	outputStream.Write (FileVersion);
}

const ApplicationHeaderIO appHeaderIO;
