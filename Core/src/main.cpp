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
	Clear();
	Point points[3] = {
		Point(-0.90f, -0.90f),
		Point(0.85f, -0.90f),
		Point(-0.90f,  0.85f)
	};
	Triangle* tri = new Triangle;//memory leak
	tri->SetPoints(points);
	AddTriangle(tri);

}

void App::Update()
{

}

App::~App()
{

}

int main()
{
	unitTest();
	return 0;
}