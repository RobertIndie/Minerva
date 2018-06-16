#include "renderer.h"
#include <sstream>
#include <iostream>

using namespace std;

list<GLFWwindow*> Renderer::windowsPool;

void Renderer::Initialize()
{
	MODULE_START("Renderer");
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
	addWindow(_window);

	glfwMakeContextCurrent(_window);
	glfwSetWindowCloseCallback(_window, windowCloseCallback);
	glfwSetKeyCallback(_window, keyCallback);

	gl3wInit();

	isInited = true;
	VAO = new GLuint;
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	GLfloat  vertices[6][2] = {
		{ -0.90f, -0.90f },{ 0.85f, -0.90f },{ -0.90f,  0.85f },  // Triangle 1
	{ 0.90f, -0.85f },{ 0.90f,  0.90f },{ -0.85f,  0.90f }   // Triangle 2
	};
	buffer = new GLuint;
	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLfloat  obs[6][2] = {
		{ -0.90f, -0.90f },{ 0.85f, -0.90f },{ -0.90f,  0.85f },  // Triangle 1
	{ 0.90f, -0.85f },{ 0.90f,  0.90f },{ -0.85f,  0.90f }   // Triangle 2
	};
	GLuint* ob = new GLuint;
	glGenBuffers(1, ob);
	glBindBuffer(GL_ARRAY_BUFFER, *ob);
	glBufferData(GL_ARRAY_BUFFER, sizeof(obs), obs, GL_STATIC_DRAW);

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER, "../../../shader/shader.vert" },
		{ GL_FRAGMENT_SHADER, "../../../shader/shader.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);
#define BUFFER_OFFSET(a) ((void*)(a))
	glVertexAttribPointer(0, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	MODULE_END("Renderer");

	
}

void Renderer::Run()
{
	if (!isInited)
	{
		errorInput("Cannot run the renderer before initialize it.");
	}
	while (!glfwWindowShouldClose(_window))
	{
		glfwSwapBuffers(_window);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(*VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

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

void Renderer::addWindow(GLFWwindow* window)
{
	windowsPool.push_back(window);
	stringstream ss;
	ss << "Add window{" << window << "} to windows pool.";
	debugOutput(ss.str().c_str());
}

void Renderer::destroyWindow(GLFWwindow * window)
{
	windowsPool.remove(window);
	glfwDestroyWindow(window);
	window = NULL;
}

Renderer::~Renderer()
{
	destroyWindow(_window);
	if (windowsPool.size() == 0)
		glfwTerminate();
	debugOutput("Terminate renderer");
}