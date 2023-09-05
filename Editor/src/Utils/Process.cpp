#include "epch.h"
#include "Process.h"

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

Process::Process(const std::string& processPath)
	: m_ProcessPath(processPath)
{
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

    std::string line;
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        line += buffer.data();
        line = line.substr(0, line.size() - 1);
        if (!line.empty())
            result.push_back(line);
        line = "";
    }
    return result;
}
