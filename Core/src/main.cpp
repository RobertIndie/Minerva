/*
	Studio: Ambicour Studio
	Author: Aaron Robert
*/
#include "main.h"
#include "debug.h"

#include "GLFW/glfw3.h"
#include "unitTest.h"

using namespace std;

void App::Initialize()
{
	base::Initialize();
}

void App::Update()
{
	Clear();
	Point points[3] = {
		Point(-0.90f, -0.90f),
		Point(0.85f, -0.90f),
		Point(-0.90f,  0.85f)
	};
	Triangle tri;
	tri.SetPoints(points);
	DrawTriangle(&tri);
}

App::~App()
{

}

int main()
{
	unitTest();
	App* app = new App;
	app->Initialize();
	app->Run();
	delete app;
	app = NULL;
	return 0;
}