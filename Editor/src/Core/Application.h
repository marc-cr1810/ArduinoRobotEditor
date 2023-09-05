#pragma once

#include "Core/Project.h"
#include "Editors/CodeEditor/CodeEditor.h"

#include "Windows/ProjectExplorer.h"
#include "Windows/OutputWindow.h"

struct GLFWwindow;
struct ImGuiWindow;

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

	bool SetActiveEditor();
	bool SetActiveEditor(Ref<Editor> editor);
	bool FindActiveEditor();
	bool SetActiveEditorFromWindow(ImGuiWindow* window);

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
	bool m_InitEditorsWindow = false;

	GLFWwindow* m_Window;

	// Windows
	Ref<ProjectExplorer> m_ProjExp;
	Ref<OutputWindow> m_OutputWind;

	// Editors
	Ref<Editor> m_ActiveEditor;
	std::vector<Ref<Editor>> m_Editors;
	bool m_RemovedEditor = false;
	bool m_GetNewEditor = false;

	// Project
	Ref<Project> m_Project;
private:
	static Application* s_Instance;
};