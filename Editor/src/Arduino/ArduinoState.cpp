#include "epch.h"
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
			ProcessCompilationResult();
			m_State = ARDUINO_STATE_READY;
			break;
		}
		case ARDUINO_STATE_UPLOADING:
		{
			auto result = m_Proc->GetData();
			for each (auto s in result)
				Application::Get().OutputLog("Upload", s);
			Application::Get().OutputLog("Upload", utils::string_format("========== Upload finished =========="));
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
	Application::Get().OutputLog("Build", "Build started...");
	Application::Get().OutputLog("Build", utils::string_format("------ Build Started: Project: %s ------", project->GetName().c_str()));
	std::string compileCmd = utils::string_format("compile %s -b %s --no-color", project->GetDirectory().c_str(), connection.FQBN.c_str());
	m_Proc->Start(compileCmd);
}

void ArduinoState::Upload(const ArduinoConnection& connection, const Ref<Project> project)
{
	m_State = ARDUINO_STATE_UPLOADING;
	Application::Get().ClearOutput("Upload");
	Application::Get().OutputLog("Upload", utils::string_format("------ Upload Started: Project: %s ------", project->GetName().c_str()));
	std::string uploadCmd = utils::string_format("upload %s -p %s --no-color", project->GetDirectory().c_str(), connection.Port.c_str());
	m_Proc->Start(uploadCmd);
}

void ArduinoState::FindAvaliablePorts()
{
	m_Connections = ArduinoConnections::GetConnections();
}

static void GetErrors(const std::vector<std::string>& lines)
{
	std::unordered_map< std::string, std::map<int, std::string>> errors;
	for (int i = 0; i < lines.size(); i++)
	{
		std::string line = lines[i];

		if (line.rfind("Used platform", 0) == 0)
			break;

		int colonPos = line.find_first_of(':', 2);
		if (colonPos == std::string::npos)
			continue;

		std::string file = line.substr(0, colonPos);
		int firstQuotePos = line.find_first_of('\'', colonPos);
		int lastQuotePos = line.find_first_of("\':", firstQuotePos + 1);
		std::string locationStr = lastQuotePos != std::string::npos ? line.substr(firstQuotePos, lastQuotePos - firstQuotePos) : line.substr(firstQuotePos);
		while (lastQuotePos == std::string::npos)
		{
			line = lines[++i];
			lastQuotePos = line.find("\':");
			locationStr += lastQuotePos != std::string::npos ? line.substr(0, lastQuotePos) : line;
		}

		line = lines[++i];
		int errorTextPos = line.find("error:");
		std::string errorPosStr = line.substr(colonPos + 1, (errorTextPos - colonPos) - 2);
		int separatorPos = errorPosStr.find(':');
		int linePos = std::stoi(errorPosStr.substr(0, separatorPos));
		int colPos = std::stoi(errorPosStr.substr(separatorPos + 1));

		std::string errorReason = line.substr(errorTextPos + 7);
		line = lines[++i];
		while (line.rfind("   ", 0) != 0)
		{
			errorReason += " " + line;
			line = lines[++i];
		}

		std::pair<int, std::string> errorResult = std::make_pair(linePos, errorReason);
		errors[file].insert(errorResult);
		line = lines[++i];

		Application::Get().OutputLog("Build", utils::string_format("%s(%i,%i): error: %s", file.c_str(), linePos, colPos, errorReason.c_str()));
	}

	for (auto& [key, value] : errors)
	{
		Application::SetErrors(key, value);
	}
}

void ArduinoState::ProcessCompilationResult()
{
	bool buildSuccess = true;
	auto result = m_Proc->GetData();

	if (result.size() > 0)
	{
		if (result[result.size() - 1].rfind("Error during build:", 0) == 0)
		{
			GetErrors(result);
			buildSuccess = false;
		}
		else
		{
			for (int i = 0; i < result.size(); i++)
			{
				std::string s = result[i];
				Application::Get().OutputLog("Build", s);
			}
		}
	}

	Application::Get().OutputLog("Build", utils::string_format("Done building project \"%s\" -- %s",
		Application::GetProject()->GetName().c_str(),
		buildSuccess ? "SUCCESS" : "FAILED"));
	int successCount = buildSuccess ? 1 : 0;
	int failCount = buildSuccess ? 0 : 1;
	Application::Get().OutputLog("Build", utils::string_format("========== Build %i succeeded, %i failed ==========", successCount, failCount));
}
