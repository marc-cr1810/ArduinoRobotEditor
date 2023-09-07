#pragma once

class Project
{
public:
	Project(const std::string& filepath);
	~Project() = default;

	std::string GetFilepath() const { return m_Filepath; }
	std::string GetDirectory() const { return m_Directory; }
	std::string GetName() const { return m_Name; }

	void SetStartupFile(const std::string& file) { m_StartupFile = file; }

	bool FileIsIncluded(const std::string& file);
	void IncludeFile(const std::string& file);
	void ExcludeFile(const std::string& file);

	bool IsLoaded() const { return m_Loaded; }
private:
	std::string m_Filepath;
	std::string m_Name;
	std::string m_Directory;

	std::string m_StartupFile;
	std::vector<std::string> m_IncludeFiles;

	bool m_Loaded = false;
	
	friend class ProjectExplorer;
	friend class ProjectSerializer;
};