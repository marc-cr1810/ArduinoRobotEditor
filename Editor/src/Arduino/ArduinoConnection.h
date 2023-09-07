#pragma once

#include <string>
#include <vector>

struct ArduinoConnection
{
	std::string Port;
	std::string Protocol;
	std::string Type;
	std::string BoardName;
	std::string FQBN;
	std::string Core;
};

class ArduinoConnections final
{
public:
	static std::vector<std::string> GetConnections();
};