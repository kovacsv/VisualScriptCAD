#ifndef USERSETTINGS_HPP
#define USERSETTINGS_HPP

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

public:
	RenderSettings	renderSettings;

private:
	std::string xmlFilePath;
};

#endif
