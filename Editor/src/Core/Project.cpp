#include "epch.h"
#include "Project.h"

#include "Core/ProjectSerializer.h"

Project::Project(const std::string& filepath)
	: m_Filepath(filepath)
{
	std::string filename = std::filesystem::path(filepath).filename().string();
	size_t lastIndex = filename.find_last_of(".");
	m_Name = filename.substr(0, lastIndex);
	m_Directory = std::filesystem::path(m_Filepath).parent_path().string();

	LOG_INFO("Loading project {0}...", m_Name);

	ProjectSerializer::Deserialize(this, m_Filepath);

	LOG_INFO("Loaded project!");
	m_Loaded = true;
}

bool Project::FileIsIncluded(const std::string& file)
{
	if (std::find(m_IncludeFiles.begin(), m_IncludeFiles.end(), "abc") == m_IncludeFiles.end())
		return false;
	return true;
}

void Project::IncludeFile(const std::string& file)
{
	if (FileIsIncluded(file))
	{
		LOG_WARN("File {0} already is included in the project", file);
		return;
	}

	m_IncludeFiles.push_back(file);
}

void Project::ExcludeFile(const std::string& file)
{
	if (!FileIsIncluded(file))
	{
		LOG_WARN("File {0} already is not included in the project", file);
		return;
	}

	auto itr = std::find(m_IncludeFiles.begin(), m_IncludeFiles.end(), file);
	if (itr != m_IncludeFiles.end())
		m_IncludeFiles.erase(itr);
}
