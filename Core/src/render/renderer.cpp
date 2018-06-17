#include "renderer.h"
#include <sstream>
#include <iostream>

using namespace std;

#pragma region Primitives

Point::Point()
{
	verticesCount = VERTICES_COUNT;
	vertices = new float[VERTICES_COUNT];
}

Point::Point(const float x, const float y)
{
	verticesCount = VERTICES_COUNT;
	vertices = new float[VERTICES_COUNT];
	*vertices = x;
	*(vertices + 1) = y;
}

Triangle::Triangle()
{
	verticesCount = VERTICES_COUNT;
	vertices = new float[VERTICES_COUNT];
}

void
Triangle::SetPointPosition(const Point* points)
{
	float* tmp = vertices;
	for (int i = 0; i < VERTICES_COUNT / Point::VERTICES_COUNT; ++i)
	{
		*(tmp++) = (points + i)->GetX();
		*(tmp++) = (points + i)->GetY();
	}
}

Point
Triangle::GetPoint(const int index) const
{
	return Point(*(vertices + 2 * index), *(vertices + 2 * index + 1));
}
#pragma endregion


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

	//GLfloat  vertices[6][2] = {
	//	{ -0.90f, -0.90f },{ 0.85f, -0.90f },{ -0.90f,  0.85f },  // Triangle 1
	//{ 0.90f, -0.85f },{ 0.90f,  0.90f },{ -0.85f,  0.90f }   // Triangle 2
	//};
	buffer = new GLuint;
	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
	glEnableVertexAttribArray(0);

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

		glBufferData(GL_ARRAY_BUFFER, vertices.size * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		//Draw Triangles
		int primitiveCount = vertices.size % Triangle::VERTICES_COUNT;
		if (primitiveCount != 0)
			errorInput("vertices count is wrong!");
		glDrawArrays(GL_TRIANGLES, 0, primitiveCount);

		Update();
		glfwPollEvents();
	}
}

void
Renderer::DrawTriangle(const Triangle* triangle)
{
	const float* triangleVertices = triangle->GetVertices();
	vertices.insert(vertices.end(), triangleVertices, triangleVertices + Triangle::VERTICES_COUNT);
}

void
Renderer::Clear()
{
	vertices.clear();
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
	Clear();
	destroyWindow(_window);
	if (windowsPool.size() == 0)
		glfwTerminate();
	debugOutput("Terminate renderer");
}