#include "epch.h"
#include "ProjectSerializer.h"

#include "Utils/YAML.h"

ProjectSerializer::ProjectSerializer(const Ref<Project>& project)
	: m_Project(project)
{
}

void ProjectSerializer::Serialize(const std::string& filepath)
{	
	YAML_BEGIN();

	YAML_KEY_VALUE("Name",  m_Project->m_Name);
	YAML_KEY_VALUE("StartupFile", m_Project->m_StartupFile);
	YAML_KEY("IncludeFiles");
	YAML_BEGIN_MAP();
	{
		for each (auto include in m_Project->m_IncludeFiles)
		{
			YAML_KEY_VALUE("IncludeFile", include);
		}
	}
	YAML_END_SEQ();

	YAML_END();
}

bool ProjectSerializer::Deserialize(const std::string& filepath)
{
	return Deserialize(m_Project.get(), filepath);
}

bool ProjectSerializer::Deserialize(Project* project, const std::string& filepath)
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
		LOG_ERROR("Failed to deserialize project '{0}'\n     {1}", filepath, ex.what());
		return false;
	}

	if (!data["Name"])
		return false;

	project->m_Name = data["Name"].as<std::string>();
	project->m_StartupFile = data["StartupFile"].as<std::string>();

	auto includeFiles = data["IncludeFiles"];
	if (includeFiles)
	{
		for (YAML::const_iterator it = includeFiles.begin(); it != includeFiles.end(); ++it)
		{
			project->m_IncludeFiles.push_back(it->second.as<std::string>());
		}
	}

	return true;
}
