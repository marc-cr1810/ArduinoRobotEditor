#pragma once
#include "Core/Project.h"

#include "Windows/Window.h"

class ProjectExplorer : public Window
{
public:
	ProjectExplorer(Ref<Project> project);

	void OnRender() override;
private:
	Ref<Project> m_Project;
};