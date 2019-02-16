#ifndef USERSETTINGS_HPP
#define USERSETTINGS_HPP

#include "Export.hpp"

#include <vector>
#include <string>

enum class ViewMode
{
	Lines,
	Polygons
};

enum class AxisMode
{
	On,
	Off
};

class RenderSettings
{
public:
	RenderSettings (ViewMode viewMode, AxisMode axisMode);

	ViewMode viewMode;
	AxisMode axisMode;
};

class ExportSettings
{
public:
	ExportSettings (Modeler::FormatId format, const std::wstring& folder, const std::wstring& name);

	Modeler::FormatId	format;
	std::wstring		folder;
	std::wstring		name;
};

class UserSettings
{
public:
	UserSettings ();

	void	Load ();
	void	Save ();

	void	AddRecentFile (const std::wstring& filePath);

	RenderSettings				renderSettings;
	ExportSettings				exportSettings;
	std::vector<std::wstring>	recentFiles;
	bool						isMaximized;
};

#endif
