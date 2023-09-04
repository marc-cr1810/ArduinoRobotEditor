#pragma once

#include "Core/Project.h"

class ProjectSerializer
{
public:
	ProjectSerializer(const Ref<Project>& project);

	void Serialize(const std::string& filepath);
	bool Deserialize(const std::string& filepath);
	static bool Deserialize(Project* project, const std::string& filepath);
private:
	Ref<Project> m_Project;
};