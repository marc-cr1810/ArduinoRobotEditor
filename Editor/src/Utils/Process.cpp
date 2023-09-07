#include "epch.h"
#include "Process.h"

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include "Core/Application.h"
#include "Utils/Utils.h"

Process::Process(const std::string& processPath)
	: m_ProcessPath(processPath)
{
}

void Process::Start(const std::string& arguments)
{
    StartInfo startInfo = {
        arguments
    };
    Start(startInfo);
}

void Process::Start(const StartInfo startInfo)
{
    using namespace std::chrono_literals;

    m_Status = PROCESS_STATUS_RUNNING;

    m_Data.clear();
    m_StartInfo = startInfo;

    m_Thread = std::async(std::launch::async, [this] {
        std::string cmd = m_StartInfo.Arguments + (m_StartInfo.GetErrorMsg ? " 2>&1" : "");
        Exec(cmd.c_str());
        LOG_INFO("The thread {0} has exited with code {1}", (uint64_t)(&m_Thread), m_ReturnCode);
        Application::Get().OutputLog("General", utils::string_format("The thread %i has exited with code %i", (uint64_t)(&m_Thread), m_ReturnCode), false);
    });
}

std::pair<std::vector<std::string>, int> Process::Exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::vector<std::string> result;
    int returnCode = -1;
    auto pcloseWrapper = [&returnCode](FILE* cmd) { returnCode = _pclose(cmd); };

    std::string execCmd = "\"" + m_ProcessPath + "\" " + cmd;
    std::unique_ptr<FILE, decltype(pcloseWrapper)> pipe(_popen(execCmd.c_str(), "r"), pcloseWrapper);

    if (!pipe) {
        throw std::runtime_error("_popen() failed!");
    }

    m_Status = PROCESS_STATUS_RUNNING;

    std::string line;
    while (!feof(pipe.get()) && !m_Cancel)
    {
        // use buffer to read and add to result
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
        {
            line += buffer.data();
            line = line.substr(0, line.size() - 1);
            if (!line.empty())
                result.push_back(line);
            line = "";
        }
    }
    m_Data = result;
    m_Status = PROCESS_STATUS_READY;
    return std::make_pair(result, returnCode);
}
