﻿/*
	Studio: Ambicour Studio
	Author: Aaron Robert
*/
#include "main.h"
#include "debug.h"

#include "GLFW/glfw3.h"

using namespace std;

void App::Initialize()
{
	base::Initialize();
}

void App::Update()
{

}

App::~App()
{

}

int main()
{
	App* app = new App;
	app->Initialize();
	app->Run();
	delete app;
	app = NULL;
	return 0;
}