#pragma once

#include <list>

#include "GLFW/glfw3.h"
#include "debug.h"

class Renderer
{
public:
	Renderer(void) {}
	virtual ~Renderer(void);
	virtual void Initialize();
	virtual void Update() = 0;
	void Run();
protected:
	GLFWwindow* _window;
	static std::list<GLFWwindow*> _windowsPool;
private:
	static void errorCallback(int error, const char* description);
	static void windowCloseCallback(GLFWwindow* window);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void addWidnow(GLFWwindow* window);
};