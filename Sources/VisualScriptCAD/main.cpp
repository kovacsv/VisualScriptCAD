#include <wx/wx.h>
#include "NE_Debug.hpp"
#include "MainWindow.hpp"

#ifdef DEBUG
#pragma comment(lib, "NodeEngineDebug.lib")
#pragma comment(lib, "NodeUIEngineDebug.lib")
#pragma comment(lib, "BuiltInNodesDebug.lib")
#pragma comment(lib, "WindowsAppSupportDebug.lib")
#pragma comment(lib, "wxWidgetsAppSupportDebug.lib")
#else
#pragma comment(lib, "NodeEngine.lib")
#pragma comment(lib, "NodeUIEngine.lib")
#pragma comment(lib, "BuiltInNodes.lib")
#pragma comment(lib, "WindowsAppSupport.lib")
#pragma comment(lib, "wxWidgetsAppSupport.lib")
#endif

class Application : public wxApp
{
public:
	Application () :
		mainWindow (nullptr)
	{
		EnableLeakDetection ();
	}

	virtual bool OnInit ()
	{
		wxImage::AddHandler (new wxPNGHandler ());
		std::wstring defaultFileName;
		if (argc == 2) {
			defaultFileName = wxString (argv[1]).ToStdWstring ();
		}
		mainWindow = new MainWindow (defaultFileName);
		mainWindow->Show (true);
		return true;
	}

private:
	MainWindow* mainWindow;
};

IMPLEMENT_APP (Application)
