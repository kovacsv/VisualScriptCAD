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
	RenderSettings ();
	RenderSettings (ViewMode viewMode, AxisMode axisMode, double gridSize, int gridCount);

	ViewMode	viewMode;
	AxisMode	axisMode;
	double		gridSize;
	int			gridCount;
};

class ImageSettings
{
public:
	ImageSettings ();
	ImageSettings (int width, int height, int multisampling);

	bool IsValid () const;

	int width;
	int height;
	int multisampling;
};

class ExportSettings
{
public:
	enum class FormatId
	{
		Obj = 0,
		Stl = 1,
		Off = 2,
		Png = 3
	};

	ExportSettings ();
	ExportSettings (FormatId format, const std::wstring& folder, const std::wstring& name);

	FormatId		format;
	ImageSettings	image;
	std::wstring	folder;
	std::wstring	name;
};

class UserSettings
{
public:
	UserSettings ();

	void	Load ();
	void	Save ();

	void	AddRecentFile (const std::wstring& filePath);
	void	RemoveRecentFile (const std::wstring& filePath);

	RenderSettings				renderSettings;
	ExportSettings				exportSettings;
	std::vector<std::wstring>	recentFiles;
	bool						isMaximized;
};

#endif
