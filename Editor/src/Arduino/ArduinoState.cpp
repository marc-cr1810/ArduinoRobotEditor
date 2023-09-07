#include "ArduinoState.h"

#include "Core/Application.h"
#include "Utils/Utils.h"

ArduinoState::ArduinoState()
{
	m_Proc = CreateRef<Process>(Application::GetSettings()->ArduinoCLIPath);
}

void ArduinoState::OnUpdate()
{
	if (!m_Proc->IsRunning() && !IsReady())
	{
		switch (m_State)
		{
		case ARDUINO_STATE_COMPILING:
		{
			auto result = m_Proc->GetData();
			for each (auto s in result)
				Application::Get().OutputLog("Build", s);
			Application::Get().OutputLog("Build", utils::string_format("========== Build finished =========="));
			m_State = ARDUINO_STATE_READY;
			break;
		}
		case ARDUINO_STATE_UPLOADING:
		{
			auto result = m_Proc->GetData();
			for each (auto s in result)
				Application::Get().OutputLog("Upload", s);
			m_State = ARDUINO_STATE_READY;
			break;
		}
		}
	}
}

void ArduinoState::Compile(const ArduinoConnection& connection, const Ref<Project> project)
{
	m_State = ARDUINO_STATE_COMPILING;
	Application::Get().ClearOutput("Build");
	Application::Get().OutputLog("Build", utils::string_format("------ Build Started: Project: %s ------", project->GetName().c_str()));
	std::string compileCmd = utils::string_format("compile %s -b %s", project->GetDirectory().c_str(), connection.FQBN.c_str());
	m_Proc->Start(compileCmd);
}

void ArduinoState::Upload(const ArduinoConnection& connection, const Ref<Project> project)
{
	m_State = ARDUINO_STATE_UPLOADING;
}
