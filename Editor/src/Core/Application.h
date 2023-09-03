#pragma once

#include "Editors/CodeEditor/CodeEditor.h"

struct GLFWwindow;

class Application
{
public:
	Application();
	~Application();

	void Run();
private:
	void OnUpdate();
	void OnRender();

	void RenderMenuBar();
private:
	int m_Width;
	int m_Height;

	GLFWwindow* m_Window;

	Ref<Editor> m_Editor;
};