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
        Exec(m_StartInfo.Arguments.c_str());
        LOG_INFO("The thread {0} has exited", (uint64_t)(&m_Thread));
        Application::Get().OutputLog("General", utils::string_format("The thread %i has exited", (uint64_t)(&m_Thread)), false);
    });
}

std::vector<std::string> Process::Exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::vector<std::string> result;

    std::string execCmd = "\"" + m_ProcessPath + "\" " + cmd;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(execCmd.c_str(), "r"), _pclose);

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
    return result;
}
