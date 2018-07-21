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
	float* vertices;
	std::size_t indicesCount;//TODO: using index drawing
	float* indices;
public:
	bool enableIndex = false;
	virtual std::size_t GetVerticesCount() const
	{
		return verticesCount;
	}
	virtual const float* GetVertices() const
	{
		return vertices;
	}
	virtual std::size_t GetIndicesCount() const
	{
		return indicesCount;
	}
	virtual const float* GetIndices() const
	{
		return indices;
	}
	virtual void PreRender() {};
	~Primitive();
};

class Point :public Primitive
{
#define GET_X *(vertices)
#define GET_Y *(vertices + 1)
#define GET_Z *(vertices + 2)
private:
public:
	typedef class Primitive base;
	const static std::size_t VERTICES_COUNT = 3;
	Point(const float x = 0, const float y = 0, const float z = 0);
	float GetX() const
	{
		return GET_X;
	}
	float GetY() const
	{
		return GET_Y;
	}
	float GetZ() const
	{
		return GET_Z;
	}
	Point* SetX(const float x)
	{
		GET_X = x;
		return this;
	}
	Point* SetY(const float y)
	{
		GET_Y = y;
		return this;
	}
	Point* SetZ(const float z)
	{
		GET_Z = z;
		return this;
	}
	bool operator ==(const Point &p) const
	{
		return Equal(GET_X, p.GetX()) && Equal(GET_Y, p.GetY()) && Equal(GET_Z, p.GetZ());
	}
	~Point();
#undef GET_X
#undef GET_Y
#undef GET_Z
};

class Triangle :public Primitive
{
public:
	typedef class Primitive base;
	const static std::size_t VERTICES_COUNT = 3 * Point::VERTICES_COUNT;
	Triangle();
	void SetPoints(const Point*);
	Point GetPoint(const int index) const;
	~Triangle();
};

class Rectangle :public Primitive //TODO: implement rectangle primitive
{
private:
	float _width;
	float _height;
	Point _pos;
public:
	typedef class Primitive base;
	const static std::size_t VERTICES_COUNT = 4 * Point::VERTICES_COUNT;
	Rectangle();
	void SetWidth(const float width);
	float GetWidth() const;
	void SetHeight(const float height);
	float GetHeight() const;
	void SetPosition(const Point pos);
	Point GetPosition() const;
	void PreRender() override;
};

class Renderer
{
public:
	std::vector<Primitive*> primitives;
	Renderer(void) {}
	virtual ~Renderer(void);
	virtual void Initialize();
	virtual void Update();
	virtual void Resize(int width, int height);
	void Run();
	void AddTriangle(const Triangle* triangle);
	void Clear();

	unsigned int app_time();

protected:
	GLFWwindow* _window;
private:
	std::vector<float> vertices;
	static std::list<Renderer*> renderersPool;
	bool isInited = false;

#ifdef _WIN32
	ULONGLONG       m_appStartTime;
#else
	struct timeval  m_appStartTime;
#endif

	ShaderProgram3D* shaderProgram;
	float viewportAspect = 0.0f;
	static void errorCallback(int error, const char* description);
	static void windowCloseCallback(GLFWwindow* window);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void windowSzieCallback(GLFWwindow* window, int width, int height);
	static void addRenderer(Renderer* renderer);
	static void removeRenderer(Renderer* renderer);
};
