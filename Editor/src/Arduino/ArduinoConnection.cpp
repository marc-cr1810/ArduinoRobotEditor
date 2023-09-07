#include "epch.h"
#include "ArduinoConnection.h"

#include "Core/Application.h"
#include "Utils/Process.h"

std::vector<std::string> ArduinoConnections::GetConnections()
{
	Process proc(Application::GetSettings()->ArduinoCLIPath);
	auto result = proc.Exec("board list");

	return result;
}
