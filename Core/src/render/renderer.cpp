#include "renderer.h"
#include <sstream>
#include <iostream>

using namespace std;

std::list<Renderer*> Renderer::renderersPool;

#pragma region Primitives

Primitive::~Primitive()
{
	delete[] vertices;
	vertices = NULL;
}

Point::Point()
{
	verticesCount = VERTICES_COUNT;
	vertices = new float[VERTICES_COUNT];
	*vertices = 0;
	*(vertices + 1) = 0;
}

Point::Point(const float x, const float y)
{
	verticesCount = VERTICES_COUNT;
	vertices = new float[VERTICES_COUNT];
	*vertices = x;
	*(vertices + 1) = y;
}

Point::~Point()
{

}

Triangle::Triangle()
{
	verticesCount = VERTICES_COUNT;
	vertices = new float[VERTICES_COUNT];
}

Triangle::~Triangle()
{

}

void
Triangle::SetPoints(const Point* points)
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

Rectangle::Rectangle()
{

}

void
Rectangle::SetWidth(const float width)
{
	_width = width;
}

float
Rectangle::GetWidth() const
{
	return _width;
}

void
Rectangle::SetHeight(const float height)
{
	_height = height;
}

float
Rectangle::GetHeight() const
{
	return _height;
}

void
Rectangle::SetPosition(const Point pos)
{
	_pos = pos;
}

Point
Rectangle::GetPosition() const
{
	return _pos;
}

void
Rectangle::PreRender()
{
	vector<float> tmp;
	tmp.insert
}
#pragma endregion

std::list<Renderer*> renderersPool;

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
	addRenderer(this);

	glfwMakeContextCurrent(_window);
	glfwSetWindowCloseCallback(_window, windowCloseCallback);
	glfwSetKeyCallback(_window, keyCallback);
	glfwSetWindowSizeCallback(_window, windowSzieCallback);

	gl3wInit();

	isInited = true;
	VAO = new GLuint;
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	buffer = new GLuint;
	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER, "../../../shader/shader.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "../../../shader/shader.fs.glsl" },
		{ GL_NONE, NULL }
	};

	shaderProgram = LoadShaders(shaders);
	glUseProgram(shaderProgram);
#define BUFFER_OFFSET(a) ((void*)(a))
	glVertexAttribPointer(0, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	MODULE_END("Renderer");

	
}

void
Renderer::Update()
{
	return;
}

void
Renderer::Resize(int width, int height)
{
	glViewport(0, 0, width, height);
	viewportAspect = float(height) / float(width);
	return;
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

		if (vertices.size() != 0)
		{
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

			//Draw Triangles
			int pointsCount = vertices.size() / Point::VERTICES_COUNT;
			if (vertices.size() % Triangle::VERTICES_COUNT != 0)//check if vertices number matches the triangle number
				errorInput("vertices count is wrong!");
			glDrawArrays(GL_TRIANGLES, 0, vertices.size() / Point::VERTICES_COUNT);
		}
		

		Update();
		glfwPollEvents();
	}
}

void
Renderer::DrawTriangle(const Triangle* triangle)//TODO
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
	errorInput(s.str().c_str());
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

void
Renderer::windowSzieCallback(GLFWwindow* window, int width, int height)
{
	list<Renderer*>::iterator iter = renderersPool.begin();
	for (; iter != renderersPool.end(); iter++)
	{
		if ((*iter)->_window == window)
			break;
	}
	if (iter == renderersPool.end())
	{
		stringstream s;
		s << "Cannot find window {" << window << "}" << endl;
		errorInput(s.str().c_str());
	}
	(*iter)->Resize(width, height);
}

void Renderer::addRenderer(Renderer* renderer)
{
	renderersPool.push_back(renderer);
	stringstream ss;
	ss << "Add renderer{" << renderer << "} to renderers pool." << "Window:{" << renderer->_window << "}";
	debugOutput(ss.str().c_str());
}

void 
Renderer::removeRenderer(Renderer* renderer)
{
	renderersPool.remove(renderer);
}

Renderer::~Renderer()
{
	glUseProgram(0);
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(1, buffer);
	Clear();
	delete VAO;
	VAO = NULL;
	delete buffer;
	buffer = NULL;
	removeRenderer(this);
	glfwDestroyWindow(_window);
	_window = NULL;
	if (renderersPool.size() == 0)
		glfwTerminate();
	debugOutput("Terminate renderer");
}