#pragma once

#include <string>

#include "Core/Base.h"

#define APP_SETTINGS_PATH_PROJ_DEFAULT		"Projects"
#define APP_SETTINGS_PATH_ARDINOCLI_DEFAULT	"arduino-cli.exe"

struct Settings
{
	std::string ProjectsPath;
	std::string ArduinoCLIPath;
};

class SettingsSerializer
{
public:
	SettingsSerializer(Ref<Settings> settings);

	void Serialize(const std::string& filepath);
	bool Deserialize(const std::string& filepath);
	static bool Deserialize(Settings* settings, const std::string& filepath);
private:
	Ref<Settings> m_Settings;
};