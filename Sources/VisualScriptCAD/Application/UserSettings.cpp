#include "UserSettings.hpp"

RenderSettings::RenderSettings (ViewMode viewMode, AxisMode axisMode) :
	viewMode (viewMode),
	axisMode (axisMode)
{

}

UserSettings::UserSettings () :
	renderSettings (ViewMode::Polygons, AxisMode::Off)
{

}
