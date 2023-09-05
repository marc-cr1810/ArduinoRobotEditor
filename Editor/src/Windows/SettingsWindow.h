#pragma once

#include "Windows/Window.h"
#include "Core/Settings.h"

class SettingsWindow : public Window
{
public:
	SettingsWindow(Ref<Settings> settings);

	void OnRender() override;
private:
	Ref<Settings> m_Settings;
};