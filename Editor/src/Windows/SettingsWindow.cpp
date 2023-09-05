#include "epch.h"
#include "SettingsWindow.h"

#include "Core/Application.h"

SettingsWindow::SettingsWindow(Ref<Settings> settings)
	: m_Settings(settings)
{
}

void SettingsWindow::OnRender()
{
	if (m_Open)
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings;
		if (ImGui::Begin("Settings", &m_Open, flags))
		{
			int length = 64;
			char* str = new char[length];

			strcpy(str, m_Settings->ProjectsPath.c_str());
			if (ImGui::InputText("Projects Path", str, length))
				m_Settings->ProjectsPath = str;

			strcpy(str, m_Settings->ArduinoCLIPath.c_str());
			if (ImGui::InputText("Arduino CLI Path", str, length))
				m_Settings->ArduinoCLIPath = str;

			delete[] str;
		}
		ImGui::End();
	}
}
