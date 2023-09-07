#pragma once

#include "Windows/Window.h"

class OutputWindow : public Window
{
public:
	enum OutputLevel
	{
		OUTPUT_LEVEL_TRACE,
		OUTPUT_LEVEL_INFO,
		OUTPUT_LEVEL_WARN,
		OUTPUT_LEVEL_ERR,
		OUTPUT_LEVEL_CRITICAL
	};
public:
	OutputWindow();

	void OnRender() override;

	void AddLog(const std::string& output, const std::string& line, bool change = false);
	void AddLog(const std::string& output, const std::string& line, OutputLevel level, bool change = false);
	void Clear(const std::string& output);
private:
	std::string m_CurrentOutput;
	std::unordered_map<std::string, std::vector<std::pair<OutputLevel, std::string>>> m_Outputs;

	bool m_ScrollToBottom = false;
};