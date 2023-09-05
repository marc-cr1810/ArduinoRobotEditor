#include "Core/Base.h"
#include "Core/Application.h"

#include "Utils/Process.h"

int main(int argc, char** argv)
{
	Log::Init();
	LOG_INFO("Hello world!");

	//Process proc("C:/Programs/arduino-cli.exe");
	//auto result = proc.Exec("board list");

	Application* app = new Application("projects/TestProject/TestProject.areproj");
	app->Run();
	delete app;

	return 0;
}