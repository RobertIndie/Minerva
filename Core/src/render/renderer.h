#pragma once

#include <list>
#include <vector>

#include <GL3/gl3w.h>
#include <GL3/gl3.h>
#include <GLFW/glfw3.h>

#include "../debug.h"
#include "shader.h"
#include "../util/util.h"

class Primitive
{
protected:
	std::size_t verticesCount;
	float* verticesPosition;
public:
	std::size_t GetVerticesCount()
	{
		return verticesCount;
	}
	const float* GetVertices()
	{
		return verticesPosition;
	}
};

class Point :public Primitive
{
#define GET_X *(verticesPosition)
#define GET_Y *(verticesPosition + 1)
private:
public:
	const static std::size_t VERTICES_COUNT = 2;
	Point();
	Point(float x, float y);
	float GetX() const
	{
		return GET_X;
	}
	float GetY() const
	{
		return GET_Y;
	}
	Point* SetX(float x)
	{
		GET_X = x;
		return this;
	}
	Point* SetY(float y)
	{
		GET_Y = y;
		return this;
	}
	bool operator ==(const Point &p) const
	{
		return Equal(GET_X, p.GetX()) && Equal(GET_Y, p.GetY());
	}
#undef GET_X
#undef GET_Y
};

class Triangle :public Primitive
{
public:
	const static std::size_t VERTICES_COUNT = 3 * Point::VERTICES_COUNT;
	Triangle();
	void SetPointPosition(Point*);
	Point GetPoint(int index);
};

class Renderer
{
public:
	std::vector<Primitive*> primitives;
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
