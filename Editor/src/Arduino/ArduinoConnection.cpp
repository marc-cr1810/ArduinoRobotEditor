#include "epch.h"
#include "ArduinoConnection.h"

#include "Core/Application.h"
#include "Utils/Process.h"

#include <map>

static std::string GetValue(const std::string& dataLine, const std::vector<std::pair<std::string, int>>& columns, const std::string& column)
{
	size_t startIndex = 0;
	for (auto const& [name, val] : columns)
	{
		if (name == column)
			return dataLine.substr(startIndex, val);
		startIndex += val + 1;
	}
	return "";
}

std::unordered_map<std::string, ArduinoConnection> ArduinoConnections::GetConnections()
{
	Process proc(Application::GetSettings()->ArduinoCLIPath);
	auto data = proc.Exec("board list");

	std::unordered_map<std::string, ArduinoConnection> result;
	if (data.size() < 2)
		return result;

	std::vector<std::pair<std::string, int>> columnSizes;
	std::string columns = data[0];
	std::string columnName = "";
	int columnSize = 0;
	for (int i = 0; i < columns.size(); i++)
	{
		char c = columns[i];
		char cNext = '\0';
		if (i < columns.size() - 1)
			cNext = columns[i + 1];

		if (c == ' ')
		{
			if (cNext != ' ' && columnName != "Board")
			{
				if (i == columns.size() - 1)
					columnSize++;
				columnSizes.push_back(std::make_pair(columnName, columnSize));
				columnName = "";
				columnSize = 0;
			}
			else
				columnSize++;
		}
		else
		{
			columnName += c;
			columnSize++;
		}
	}

	for (int i = 1; i < data.size(); i++)
	{
		std::string line = data[i];
		ArduinoConnection connection;
		connection.Port = GetValue(line, columnSizes, "Port");
		connection.Protocol = GetValue(line, columnSizes, "Protocol");
		connection.Type = GetValue(line, columnSizes, "Type");
		connection.BoardName = GetValue(line, columnSizes, "BoardName");
		connection.FQBN = GetValue(line, columnSizes, "FQBN");
		connection.Core = GetValue(line, columnSizes, "Core");

		result[connection.Port] = connection;
	}

	return result;
}
