#pragma once

#include <list>

#include <GL3/gl3w.h>
#include <GL3/gl3.h>
#include <GLFW/glfw3.h>

#include "../debug.h"
#include "shader.h"


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
	static std::list<GLFWwindow*> windowsPool;
private:
	bool isInited = false;
	GLuint* VAO;
	GLuint* buffer;
	static void errorCallback(int error, const char* description);
	static void windowCloseCallback(GLFWwindow* window);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void addWindow(GLFWwindow* window);
	static void destroyWindow(GLFWwindow* window);
};
