#include "renderer.h"
#include <iostream>

using namespace std;

std::list<Renderer*> Renderer::renderersPool;

#pragma region Primitives
//===============================================//
//					PRIMITIVE					 //
//===============================================//
Primitive::~Primitive()
{
	delete[] vertices;
	vertices = NULL;
}


//===============================================//
//					POINT   					 //
//===============================================//
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


//===============================================//
//					POINT3D 					 //
//===============================================//
Point3D::Point3D()
{
	verticesCount = VERTICES_COUNT;
	vertices = new float[VERTICES_COUNT];
	*vertices = 0;
	*(vertices + 1) = 0;
	*(vertices + 2) = 0;
}

Point3D::Point3D(const float x, const float y, const float z)
{
	verticesCount = VERTICES_COUNT;
	vertices = new float[VERTICES_COUNT];
	*vertices = x;
	*(vertices + 1) = y;
	*(vertices + 2) = z;
}

Point3D::~Point3D()
{

}


//===============================================//
//					TRIANGLE					 //
//===============================================//
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


//===============================================//
//					RECTANGLE					 //
//===============================================//
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
/*
	buffer = new GLuint;
	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);*/

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER, "../../../shader/shader.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "../../../shader/shader.fs.glsl" },
		{ GL_NONE, NULL }
	};

	shaderProgram = LoadShaders(shaders);
	glUseProgram(shaderProgram);


	render_model_matrix_loc = glGetUniformLocation(shaderProgram, "model_matrix");
	render_projection_matrix_loc = glGetUniformLocation(shaderProgram, "projection_matrix");

	// 8 corners of a cube, side length 2, centered on the origin
	static const GLfloat cube_positions[] =
	{
		-1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,
		1.0f,  1.0f, -1.0f, 1.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	// Color for each vertex
	static const GLfloat cube_colors[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	};


#define BUFFER_OFFSET(a) ((void*)(a))
	glVertexAttribPointer(0, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	Resize(640, 480);

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
#ifdef _WIN32
	m_appStartTime = ::GetTickCount64();
#else
	gettimeofday(&m_appStartTime, nullptr);
#endif

	if (!isInited)
	{
		errorInput("Cannot run the renderer before initialize it.");
	}
	while (!glfwWindowShouldClose(_window))
	{
		glfwSwapBuffers(_window);

		float t = float(app_time() & 0x1FFF) / float(0x1FFF);
		static float q = 0.0f;
		static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
		static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
		static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);

		// Setup
		//GL_CULL_FACE:specify whether front- or back-facing polygons can be culled
		//glEnable(GL_CULL_FACE);
		//GL_DEPTH_TEST:When there are other pixels in front of the current pixel, if other pixels block it, then it will not be drawed, OpenGL will only draw the frist layer.
		//glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glBindVertexArray(*VAO);


		// Activate simple shading program
		glUseProgram(shaderProgram);

		// Set up the model and projection matrix
		vmath::mat4 model_matrix(vmath::translate(0.0f, 0.0f, -5.0f) * rotate(t * 360.0f, Y) * rotate(t * 720.0f, Z));
		vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -viewportAspect, viewportAspect, 1.0f, 500.0f));

		glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
		glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix);

		glBindVertexArray(*VAO);
		
		vertices.clear();
		for (vector<Primitive*>::iterator iter = primitives.begin(); iter != primitives.end(); iter++) 
		{
			const size_t primitiveVerticesCount = (**iter).GetVerticesCount();
			const float* primitiveVertices = (**iter).GetVertices();
			vertices.insert(vertices.end(), primitiveVertices, primitiveVertices + primitiveVerticesCount);
		}

		if (vertices.size() != 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, *VAO);
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
Renderer::AddTriangle(const Triangle* triangle)
{
	primitives.push_back((Primitive*)triangle);
}

void
Renderer::Clear()
{
	vertices.clear();
}

unsigned int
Renderer::app_time()
{
#ifdef _WIN32
	ULONGLONG currentTime = ::GetTickCount64();

	return (unsigned int)(currentTime - m_appStartTime);
#else
	return 0;
#endif
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