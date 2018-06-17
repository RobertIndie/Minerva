#pragma once

#include <list>
#include <vector>

#include <GL3/gl3w.h>
#include <GL3/gl3.h>
#include <GLFW/glfw3.h>

#include "../debug.h"
#include "shader.h"

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
private:
public:
	const static std::size_t VERTICES_COUNT = 2;
	Point();
	Point(float x, float y);
	float GetX()
	{
		return *verticesPosition;
	}
	float GetY()
	{
		return *(verticesPosition + 1);
	}
	Point* SetX(float x)
	{
		*verticesPosition = x;
		return this;
	}
	Point* SetY(float y)
	{
		*(verticesPosition) = y;
		return this;
	}
};

class Triangle :public Primitive
{
public:
	const static std::size_t VERTICES_COUNT = 3 * Point::VERTICES_COUNT;
	Triangle();
	void SetPointPosition(Point*);
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
