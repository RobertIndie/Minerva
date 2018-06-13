#pragma once

#include <vector>

#include "GLFW/glfw3.h"
#include "debug.h"

class Renderer
{
public:
	Renderer(void) {}
	virtual ~Renderer(void) {}	
	virtual void Initialize();
	virtual void Display() = 0;

protected:
	GLFWwindow* _window;
	static std::vector<GLFWindow*>* _windowsPool;

private:
	static void errorCallback(int error, const char* description);
	static void windowCloseCallback(GLFWwindow* window);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};