#ifndef USERSETTINGS_HPP
#define USERSETTINGS_HPP

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

	RenderSettings	renderSettings;
};

#endif
