#pragma once

#include "Core/Base.h"
#include "Core/Project.h"
#include "Utils/Process.h"

#include "Arduino/ArduinoConnection.h"

class ArduinoState
{
public:
	enum State 
	{
		ARDUINO_STATE_READY,
		ARDUINO_STATE_COMPILING,
		ARDUINO_STATE_UPLOADING,
	};
public:
	ArduinoState();

	void OnUpdate();

	void Compile(const ArduinoConnection& connection, const Ref<Project> project);
	void Upload(const ArduinoConnection& connection, const Ref<Project> project);

	State GetState() const { return m_State; }
	bool IsReady() const { return m_State == ARDUINO_STATE_READY; }
private:
	std::vector<ArduinoConnection> m_Connections;
	State m_State = ARDUINO_STATE_READY;
	Ref<Process> m_Proc;
};