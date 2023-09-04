#pragma once

#include "Core/Project.h"
#include "Windows/ProjectExplorer.h"
#include "Editors/CodeEditor/CodeEditor.h"

struct GLFWwindow;

class Application
{
public:
	Application(const char* projFilepath);
	~Application();

	void Run();

	void OpenEditor(const char* filepath);
	void CloseEditor(Ref<Editor> editor);
private:
	void LoadTheme();

	void SetActiveEditor(Ref<Editor> editor);

	void OnUpdate();
	void OnRender();

	void RenderDockspace();

	void RenderMenuBar();
	void RenderWindows();
	void RenderEditorWindows();
public:
	static Application& Get() { return *s_Instance; }
	static Ref<Project> GetProject() { return Get().m_Project; }
	static Ref<Editor> GetEditor() { return Get().m_ActiveEditor; }
private:
	int m_Width;
	int m_Height;

	GLFWwindow* m_Window;

	// Windows
	Ref<ProjectExplorer> m_ProjExp;

	// Editors
	Ref<Editor> m_ActiveEditor;
	std::vector<Ref<Editor>> m_Editors;

	// Project
	Ref<Project> m_Project;
private:
	static Application* s_Instance;
};