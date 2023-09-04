#include "Core/Base.h"
#include "Core/Application.h"

int main(int argc, char** argv)
{
	Log::Init();
	LOG_INFO("Hello world!");

	Application* app = new Application("projects/TestProject/TestProject.areproj");
	app->Run();
	delete app;

	return 0;
}