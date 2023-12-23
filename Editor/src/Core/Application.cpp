#include "epch.h"
#include "Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_internal.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>

#include "Core/ProjectSerializer.h"
#include "Utils/Process.h"
#include "Utils/Utils.h"

#include "Editors/CodeEditor/CodeEditor.h"
#include "Editors/NodeEditor/NodeEditor.h"

Application* Application::s_Instance = nullptr;

static void GLFWErrorCallback(int error, const char* description)
{
	LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}

static void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Application::Application(const char* projFilepath)
{
	m_Width = 1024;
	m_Height = 720;

	if (!s_Instance)
		ED_ASSERT("Application instance already exists!");
	s_Instance = this;

	m_Settings = CreateRef<Settings>();
	SettingsSerializer settingsSerializer(m_Settings);
	settingsSerializer.Deserialize("settings.config");

	// Load the project
	m_Project = CreateRef<Project>(projFilepath);
	if (!m_Project->IsLoaded())
		ED_ASSERT("Failed to load project file {0}!", projFilepath);

	// Init GLFW and create the widnow
	LOG_INFO("Creating window ({0}x{1})", m_Width, m_Height);

	int success = glfwInit();
	ED_ASSERT(success, "Could not initialize GLFW!");
	glfwSetErrorCallback(GLFWErrorCallback);

#ifdef ED_DEBUG
	glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
#endif

	std::string windowTitle = m_Project->GetName() + " - Arduino Robot Editor";
	m_Window = glfwCreateWindow(m_Width, m_Height, windowTitle.c_str(), nullptr, nullptr);
	ED_ASSERT(m_Window, "Failed to create window!");
	if (!m_Window)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(0);

	// Init GLAD
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	ED_ASSERT(status, "Failed to initialize Glad!");

	glfwSetFramebufferSizeCallback(m_Window, GLFWFramebufferSizeCallback);

	// Init ImGui
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	//ImGui::StyleColorsDark();
	LoadTheme();

	// Set the default font
	float fontSize = 16.0f;
	io.Fonts->AddFontFromFileTTF("assets/fonts/CascadiaCode/ttf/CascadiaMono.ttf", fontSize);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/CascadiaCode/ttf/CascadiaMono.ttf", fontSize);

	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Init windows
	m_SettingsWindow = CreateRef<SettingsWindow>(m_Settings);
	m_ProjExp = CreateRef<ProjectExplorer>(m_Project);
	m_OutputWind = CreateRef<OutputWindow>();

	m_ProjExp->Open();
	m_OutputWind->Open();

	// Init Arduino
	m_Arduino = CreateRef<ArduinoState>();
}

Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

void Application::Run()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();

		OnUpdate();
		OnRender();
	}
}

void Application::OpenEditor(const char* filepath)
{
	for each (auto editor in m_Editors)
	{
		if (editor->GetFilepath() == filepath)
			return;
	}

	auto file = std::filesystem::path(filepath);
	auto extension = file.extension();

	Ref<Editor> editor;
	// Arduino Node file (Nodeuino!)
	if (extension == ".nino")
	{
		editor = CreateRef<NodeEditor>();
	}
	// Sort out text based files
	else
	{
		TextEditor::LanguageDefinition langDef;
		if (extension == ".ino")
			langDef = CODE_LANG_ARDUINO;
		else if (extension == ".cpp" || extension == ".hpp")
			langDef = CODE_LANG_CPP;
		else if (extension == ".c" || extension == ".h")
			langDef = CODE_LANG_C;

		editor = CreateRef<CodeEditor>(langDef);
	}

	if (editor)
	{
		editor->Edit(filepath);
		m_Editors.push_back(editor);
		SetActiveEditor(editor);
	}
}

void Application::CloseEditor(Ref<Editor> editor)
{
	auto itr = std::find(m_Editors.begin(), m_Editors.end(), editor);
	if (itr != m_Editors.end())
		m_Editors.erase(itr);
}

void Application::OutputLog(const std::string& output, const std::string& str, bool change)
{
	OutputLog(output, str, OutputWindow::OUTPUT_LEVEL_INFO, change);
}

void Application::OutputLog(const std::string& output, const std::string& str, OutputWindow::OutputLevel level, bool change)
{
	m_OutputWind->AddLog(output, str, level, change);
}

void Application::ClearOutput(const std::string& output)
{
	m_OutputWind->Clear(output);
}

void Application::LoadTheme()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 1.0f };
	colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Border
	colors[ImGuiCol_Border] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.29f };
	colors[ImGuiCol_BorderShadow] = ImVec4{ 0.0f, 0.0f, 0.0f, 0.24f };

	// Text
	colors[ImGuiCol_Text] = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
	colors[ImGuiCol_TextDisabled] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.13f, 0.13f, 0.17, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.13f, 0.13f, 0.17, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_CheckMark] = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };

	// Popups
	colors[ImGuiCol_PopupBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 0.92f };

	// Slider
	colors[ImGuiCol_SliderGrab] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.54f };
	colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.74f, 0.58f, 0.98f, 0.54f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.13f, 0.13, 0.17, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.24, 0.24f, 0.32f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.2f, 0.22f, 0.27f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

	// Scrollbar
	colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 1.0f };
	colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.24f, 0.24f, 0.32f, 1.0f };

	// Seperator
	colors[ImGuiCol_Separator] = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };
	colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };
	colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.84f, 0.58f, 1.0f, 1.0f };

	// Resize Grip
	colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.29f };
	colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.74f, 0.58f, 0.98f, 0.29f };
	colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.84f, 0.58f, 1.0f, 0.29f };

	// Docking
	colors[ImGuiCol_DockingPreview] = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };

	auto& style = ImGui::GetStyle();
	style.TabRounding = 4;
	style.ScrollbarRounding = 9;
	style.WindowRounding = 7;
	style.GrabRounding = 3;
	style.FrameRounding = 3;
	style.PopupRounding = 4;
	style.ChildRounding = 4;
}

bool Application::SetActiveEditor()
{
	ImGuiContext& g = *ImGui::GetCurrentContext();
	return SetActiveEditorFromWindow(g.NavWindow);
}

bool Application::SetActiveEditor(Ref<Editor> editor)
{
	if (editor == m_ActiveEditor)
		return false;

	if (std::find(m_Editors.begin(), m_Editors.end(), editor) == m_Editors.end())
		return false;

	if (editor)
		LOG_INFO("Set Active: {0}", editor->GetFilename());
	m_ActiveEditor = editor;
	SetEditorErrorMarkers(editor);
	return true;
}

bool Application::FindActiveEditor()
{
	ImGuiContext& g = *ImGui::GetCurrentContext();
	size_t count = g.WindowsFocusOrder.size();
	for (int i = count - 1; i >= 0; i--)
	{
		auto window = g.WindowsFocusOrder[i];
		if (!window->WasActive || !window->Active)
			continue;
		if (SetActiveEditorFromWindow(window))
			return true;
	}
	return false;
}

bool Application::SetActiveEditorFromWindow(ImGuiWindow* window)
{
	if (!window)
		return false;

	std::string name = window->Name;
	size_t typeIndex = name.find_last_of("##") + 1;
	size_t idIndex = name.find_first_of("_");
	if (typeIndex == std::string::npos || idIndex == std::string::npos)
		return false;

	std::string type = name.substr(typeIndex, idIndex - typeIndex);
	if (type != "editorWindow")
		return false;

	size_t slashIndex = name.find_first_of('/');
	auto id = name.substr(idIndex + 1, slashIndex);
	int index = std::stoi(id);
	if (m_Editors.size() > 0)
		SetActiveEditor(m_Editors[index]);
	else
		SetActiveEditor(nullptr);
	return true;
}

void Application::SetEditorErrorMarkers(Ref<Editor> editor)
{
	if (editor)
	{
		for (auto& [filepath, errors] : m_FileErrors)
		{
			auto path = std::filesystem::path(filepath);
			auto editorPathStr = GetEditor()->GetFilepath();
			std::replace(editorPathStr.begin(), editorPathStr.end(), '/', '\\');
			auto editorPath = std::filesystem::path(editorPathStr);
			if (!editorPath.is_absolute())
			{
				if (editorPathStr.front() != '\\')
				{
					editorPathStr = '\\' + editorPathStr;
					editorPath = std::filesystem::path(editorPathStr);
				}
				auto currentPath = std::filesystem::current_path();
				editorPath = currentPath.string() + editorPath.string();
			}

			if (editorPath == path)
			{
				GetEditor()->MarkErrors(errors);
				return;
			}
		}
	}
}

void Application::OnUpdate()
{
	m_Arduino->OnUpdate();

	if (m_RemovedEditor)
		m_GetNewEditor = true;

	if (m_ActiveEditor)
		m_ActiveEditor->OnUpdate();
}

void Application::OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ImGui frame
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Editor Dockspace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 270.0f;
		float minWinSizeY = style.WindowMinSize.y;
		style.WindowMinSize.y = 270.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("EditorDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;
		style.WindowMinSize.y = minWinSizeY;

		RenderDockspace();

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		if (ImGui::IsMouseClicked(0))
			SetActiveEditor();

		if (m_RemovedEditor && m_GetNewEditor)
		{
			FindActiveEditor();
			m_RemovedEditor = m_GetNewEditor = false;
		}
	}
}

void Application::RenderDockspace()
{
	RenderMenuBar();
	//RenderToolBar();
	RenderWindows();
	RenderEditorWindows();
}

void Application::RenderMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (m_ActiveEditor)
			{
				std::string filename = m_ActiveEditor->GetFilename();
				if (ImGui::MenuItem(std::string("Save " + filename).c_str(), "Ctrl+S"))
				{
					m_ActiveEditor->Save();
				}
			}
			if (m_Editors.size() > 1)
			{
				if (ImGui::MenuItem("Save All", "Ctrl+Shift+S"))
				{
					for each (auto editor in m_Editors)
						editor->Save();
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Settings"))
				m_SettingsWindow->Open();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (m_ActiveEditor)
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z", false, m_ActiveEditor->CanUndo()))
					m_ActiveEditor->Undo();
				if (ImGui::MenuItem("Redo", "Ctrl+Y", false, m_ActiveEditor->CanRedo()))
					m_ActiveEditor->Redo();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Project Explorer"))
				m_ProjExp->Open();
			if (ImGui::MenuItem("Output"))
				m_OutputWind->Open();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Build"))
		{
			bool canCompile = true;
			if (m_CurrentConnection.Port.empty())
				canCompile = false;
			if (ImGui::MenuItem("Compile", nullptr, false, canCompile))
			{
				for each (auto editor in m_Editors)
					editor->Save();
				m_Arduino->Compile(m_CurrentConnection, m_Project);
			}

			bool canUpload = true;
			for each (auto editor in m_Editors)
			{
				if (editor->IsModified())
				{
					canUpload = false;
					break;
				}
			}
			if (ImGui::MenuItem("Upload", nullptr, false, canUpload && canCompile))
			{
				m_Arduino->Upload(m_CurrentConnection, m_Project);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::BeginCombo("##ports", m_CurrentConnection.Port.c_str()))
			{
				for (auto const& [port, connection] : m_Arduino->GetAvaliablePorts())
				{
					bool isSelected = m_CurrentConnection.Port == port;
					if (ImGui::Selectable(utils::string_format("%s (%s)", port.c_str(), connection.BoardName.c_str()).c_str(), isSelected))
						m_CurrentConnection = connection;
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (ImGui::MenuItem("Reload Ports"))
			{
				m_Arduino->FindAvaliablePorts();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void Application::RenderToolBar()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	auto& colors = ImGui::GetStyle().Colors;
	const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
	const auto& buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

	ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);



	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
	ImGui::End();

}

void Application::RenderWindows()
{
	m_SettingsWindow->OnRender();
	m_ProjExp->OnRender();
	m_OutputWind->OnRender();
}

void Application::RenderEditorWindows()
{
	std::vector<std::pair<size_t, Ref<Editor>>> editorsToClose;
	ImGuiID dockspace_id = ImGui::GetID("EditorDockspace");

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
	for (int i = 0; i < m_Editors.size(); i++)
	{
		Ref<Editor> editor = m_Editors[i];
		std::string windowLabel = editor->GetFilename() + "##editorWindow_" + std::to_string(i);
		ImGuiWindowFlags editFlag = editor->IsModified() ? ImGuiWindowFlags_UnsavedDocument : 0;
		bool opened = true;
		ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		if (ImGui::Begin(windowLabel.c_str(), &opened, flags | editFlag))
		{
			ImGui::PopStyleVar();
			editor->OnRender();
		}
		else
			ImGui::PopStyleVar();
		ImGui::End();

		if (!opened)
			editorsToClose.push_back(std::make_pair(i, editor));
	}

	for each (auto editor in editorsToClose)
	{
		if (m_Editors[editor.first] == m_ActiveEditor)
			m_RemovedEditor = true;
		CloseEditor(editor.second);
	}
}

void Application::SetErrors(const std::string& filepath, std::map<int, std::string> errors)
{
	Get().m_FileErrors[filepath] = errors;
	Get().SetEditorErrorMarkers(GetEditor());
}
