#include "Core.h"

using namespace std;

void ErrorCallback(int error, const char* description)
{
	cout << "[ERROR]" << description << endl;
}

void WindowCloseCallback(GLFWwindow* window)
{

}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main()
{
	if (!glfwInit())
	{
		cout << "Init failed" << endl;
	}
	glfwSetErrorCallback(ErrorCallback);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Test window", NULL, NULL);
	if (!window)
	{
		cout << "Window creation failed" << endl;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowCloseCallback(window, WindowCloseCallback);
	glfwSetKeyCallback(window, KeyCallback);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
