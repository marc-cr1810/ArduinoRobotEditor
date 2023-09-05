#pragma once

class OutputWindow
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

	void OnRender();

	void AddLog(const std::string& output, const std::string& line, OutputLevel level = OUTPUT_LEVEL_INFO);
	void Clear(const std::string& output);

	void Open() { m_Open = true; }
private:
	std::string m_CurrentOutput;
	std::unordered_map<std::string, std::vector<std::pair<OutputLevel, std::string>>> m_Outputs;

	bool m_ScrollToBottom = false;
	bool m_Open = true;
};