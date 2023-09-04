#pragma once
#include "Core/Project.h"

class ProjectExplorer
{
public:
	ProjectExplorer(Ref<Project> project);

	void OnRender();

	void Open() { m_Open = true; }
private:
	Ref<Project> m_Project;

	bool m_Open = true;
};