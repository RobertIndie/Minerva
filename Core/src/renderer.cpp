#include "renderer.h"
#include <sstream>

using namespace std;

void Renderer::Initialize()
{
	debugOutput("Init renderer");
	if (!glfwInit())//GLFW can be initialized many times.
	{
		errorInput("GLFW init failed.");
		return;
	}
	glfwSetErrorCallback(errorCallback);

	//Create window
	_window = glfwCreateWindow(640, 480, "Test window", NULL, NULL);
	if (!_window)
	{
		errorInput("Window create failed.");
		return;
	}

	glfwMakeContextCurrent(_window);
	glfwSetWindowCloseCallback(_window, windowCloseCallback);
	glfwSetKeyCallback(_window, keyCallback);
}

void Renderer::Run()
{
	while (!glfwWindowShouldClose(_window))
	{
		Update();
		glfwPollEvents();
	}
}

void Renderer::errorCallback(int error, const char * description)
{
	stringstream s;
	s << error << ":" << description;
	errorInput("");
}

void Renderer::windowCloseCallback(GLFWwindow * window)
{

}

void Renderer::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	//just for test
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

Renderer::~Renderer()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
	debugOutput("Terminate renderer");
}