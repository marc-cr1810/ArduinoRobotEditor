#pragma once

#include "Core/Project.h"
#include "Core/Settings.h"
#include "Editors/Editor.h"

#include "Windows/SettingsWindow.h"
#include "Windows/ProjectExplorer.h"
#include "Windows/OutputWindow.h"

#include "Arduino/ArduinoState.h"

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

	void OutputLog(const std::string& output, const std::string& str, bool change = true);
	void OutputLog(const std::string& output, const std::string& str, OutputWindow::OutputLevel level, bool change = true);
	void ClearOutput(const std::string& output);
private:
	void LoadTheme();

	bool SetActiveEditor();
	bool SetActiveEditor(Ref<Editor> editor);
	bool FindActiveEditor();
	bool SetActiveEditorFromWindow(ImGuiWindow* window);

	void SetEditorErrorMarkers(Ref<Editor> editor);
	void UpdateEditorErrorMarkers(Ref<Editor> editor);

	void OnUpdate();
	void OnRender();

	void RenderDockspace();

	void RenderMenuBar();
	void RenderToolBar();
	void RenderWindows();
	void RenderEditorWindows();
public:
	static Application& Get() { return *s_Instance; }
	static Ref<Settings> GetSettings() { return Get().m_Settings; }
	static Ref<Project> GetProject() { return Get().m_Project; }
	static Ref<Editor> GetEditor() { return Get().m_ActiveEditor; }
	static void SetErrors(const std::string& filepath, std::map<int, std::string> errors);
private:
	int m_Width;
	int m_Height;
	bool m_InitEditorsWindow = false;

	GLFWwindow* m_Window;

	Ref<Settings> m_Settings;

	// Windows
	Ref<SettingsWindow> m_SettingsWindow;
	Ref<ProjectExplorer> m_ProjExp;
	Ref<OutputWindow> m_OutputWind;

	// Editors
	Ref<Editor> m_ActiveEditor;
	std::vector<Ref<Editor>> m_Editors;
	bool m_RemovedEditor = false;
	bool m_GetNewEditor = false;

	std::unordered_map<std::string, std::map<int, std::string>> m_FileErrors;

	// Project
	Ref<Project> m_Project;

	// Arduino
	Ref<ArduinoState> m_Arduino;
	ArduinoConnection m_CurrentConnection;
private:
	static Application* s_Instance;
};