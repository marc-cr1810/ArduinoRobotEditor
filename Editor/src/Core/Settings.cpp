#include "Settings.h"

#include "Utils/YAML.h"

SettingsSerializer::SettingsSerializer(Ref<Settings> settings)
	: m_Settings(settings)
{
	m_Settings->ProjectsPath = APP_SETTINGS_PATH_PROJ_DEFAULT;
	m_Settings->ArduinoCLIPath = APP_SETTINGS_PATH_ARDINOCLI_DEFAULT;
}

void SettingsSerializer::Serialize(const std::string& filepath)
{
    YAML_BEGIN();

    YAML_KEY_VALUE("ProjectsPath", m_Settings->ProjectsPath);
    YAML_KEY_VALUE("ArduinoCLIPath", m_Settings->ArduinoCLIPath);

    YAML_END();
}

bool SettingsSerializer::Deserialize(const std::string& filepath)
{
    return Deserialize(m_Settings.get(), filepath);
}

bool SettingsSerializer::Deserialize(Settings* settings, const std::string& filepath)
{
	std::ifstream stream(filepath);
	if (!stream.is_open())
		return false;
	std::stringstream strStream;
	strStream << stream.rdbuf();

	YAML::Node data;
	try
	{
		data = YAML::LoadFile(filepath);
	}
	catch (const YAML::ParserException& ex)
	{
		LOG_ERROR("Failed to deserialize settings '{0}'\n     {1}", filepath, ex.what());
		return false;
	}

	settings->ProjectsPath = data["ProjectsPath"].as<std::string>();
	settings->ArduinoCLIPath = data["ArduinoCLIPath"].as<std::string>();

    return true;
}
