#pragma once

#include <future>
#include <thread>

class Process
{
public:
	struct StartInfo
	{
		std::string Arguments;
	};
public:
	Process(const std::string& processPath);

	void Start(const std::string& arguments = "");
	void Start(const StartInfo startInfo);
	std::vector<std::string> Exec(const char* cmd);

	void Stop() { m_Cancel = true; }

	bool IsRunning() const { return m_ThreadStatus != std::future_status::ready; }
private:
	std::string m_ProcessPath;
	StartInfo m_StartInfo;

	std::vector<std::string> m_Data;

	std::future<void> m_Thread;
	std::future_status m_ThreadStatus;
	std::atomic_bool m_Cancel = ATOMIC_VAR_INIT(false);
};