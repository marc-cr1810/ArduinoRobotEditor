#include "epch.h"
#include "ProjectSerializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#define YAML_BEGIN()				YAML::Emitter out;\
									out << YAML::BeginMap

#define YAML_END()					out << YAML::EndMap;\
									std::ofstream fout(filepath);\
									fout << out.c_str();\
									fout.close()

#define YAML_KEY(key)				out << YAML::Key << key
#define YAML_KEY_VALUE(key, value)	out << YAML::Key << key << YAML::Value << value
#define YAML_BEGIN_SEQ(key)			out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq
#define YAML_END_SEQ()				out << YAML::EndSeq
#define YAML_BEGIN_MAP()			out << YAML::BeginMap
#define YAML_END_MAP()				out << YAML::EndMap

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

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
}
