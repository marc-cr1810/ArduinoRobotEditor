#pragma once

#include <future>
#include <thread>

class Process
{
public:
	struct StartInfo
	{
		std::string Arguments;
		bool GetErrorMsg = true;
	};

	enum Status
	{
		PROCESS_STATUS_READY,
		PROCESS_STATUS_RUNNING
	};
public:
	Process(const std::string& processPath);

	void Start(const std::string& arguments = "");
	void Start(const StartInfo startInfo);
	std::pair<std::vector<std::string>, int> Exec(const char* cmd);

	void Stop() { m_Cancel = true; }

	std::vector<std::string> GetData() const { return m_Data; }
	int GetReturnCode() const { return m_ReturnCode; }
	bool IsRunning() const { return m_Status == PROCESS_STATUS_RUNNING; }
private:
	std::string m_ProcessPath;
	StartInfo m_StartInfo;

	std::vector<std::string> m_Data;
	int m_ReturnCode = 0;

	std::future<void> m_Thread;
	Status m_Status;
	std::atomic_bool m_Cancel = ATOMIC_VAR_INIT(false);
};