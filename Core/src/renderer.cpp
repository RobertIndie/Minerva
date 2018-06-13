#include "renderer.h"
#include <sstream>

using namespace std;

void Renderer::Initialize()
{
	if (!glfwInit())//GLFW can be initialized many times.
	{
		errorInput("GLFW init failed.");
	}
}

void Renderer::errorCallback(int error, const char * description)
{
	stringstream s;
	s << error << ":" << description;
	errorInput(s);
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
