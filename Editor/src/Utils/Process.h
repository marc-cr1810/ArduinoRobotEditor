#pragma once

class Process
{
public:
	Process(const std::string& processPath);

	std::vector<std::string> Exec(const char* cmd);
private:
	std::string m_ProcessPath;
};