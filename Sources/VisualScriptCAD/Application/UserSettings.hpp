#ifndef USERSETTINGS_HPP
#define USERSETTINGS_HPP

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

class UserSettings
{
public:
	UserSettings ();

	void	Load ();
	void	Save ();

	void	AddRecentFile (const std::wstring& filePath);

public:
	RenderSettings				renderSettings;
	std::vector<std::wstring>	recentFiles;

private:
	std::string xmlFilePath;
};

#endif
