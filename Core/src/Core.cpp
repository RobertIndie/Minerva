/*
	Studio: Ambicour Studio
	Author: Aaron Robert
*/
#include "core.h"
#include "debug.h"

using namespace std;

void errorCallback(int error, const char* description)
{
	cout << "[ERROR]" << description << endl;
}

void windowCloseCallback(GLFWwindow* window)
{

}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main()
{
	debugOutput("Init renderer.");
	if (!glfwInit())
	{
		cout << "Init failed" << endl;
	}
	glfwSetErrorCallback(errorCallback);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Test window", NULL, NULL);
	if (!window)
	{
		cout << "Window creation failed" << endl;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowCloseCallback(window, windowCloseCallback);
	glfwSetKeyCallback(window, keyCallback);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
