#include "Core/Base.h"
#include "Core/Application.h"

#include "Utils/Utils.h"

int main(int argc, char** argv)
{
	Log::Init();

	Application* app = new Application("projects/TestProject/TestProject.areproj");
	app->Run();
	delete app;

	return 0;
}