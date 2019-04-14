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
	Application ()
	{
		EnableLeakDetection ();
	}

	virtual bool OnInit ()
	{
		if (argc == 1 || argc == 2) {
			wxInitAllImageHandlers ();
			std::wstring defaultFileName;
			if (argc == 2) {
				defaultFileName = wxString (argv[1]).ToStdWstring ();
			}
			MainWindow* mainWindow = new MainWindow (defaultFileName);
			mainWindow->Show (true);
			return true;
		}

		// make sure to exit the app correctly
		wxFrame* dummyFrame = new wxFrame (nullptr, wxID_ANY, L"");
		dummyFrame->Destroy ();

		return true;
	}
};

IMPLEMENT_APP (Application)
