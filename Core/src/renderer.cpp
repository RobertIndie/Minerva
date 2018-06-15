#include "renderer.h"
#include <sstream>
#include <iostream>

using namespace std;

list<GLFWwindow*> Renderer::windowsPool;

typedef struct {
	GLenum       type;
	const char*  filename;
	GLuint       shader;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo*);

static const GLchar*
ReadShader(const char* filename)
{
#ifdef WIN32
	FILE* infile;
	fopen_s(&infile, filename, "rb");
#else
	FILE* infile = fopen(filename, "rb");
#endif // WIN32

	if (!infile) {
#ifdef _DEBUG
		std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
		return NULL;
	}

	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	GLchar* source = new GLchar[len + 1];

	fread(source, 1, len, infile);
	fclose(infile);

	source[len] = 0;

	return const_cast<const GLchar*>(source);
}

GLuint
LoadShaders(ShaderInfo* shaders)
{
	if (shaders == NULL) { return 0; }

	GLuint program = glCreateProgram();

	ShaderInfo* entry = shaders;
	while (entry->type != GL_NONE) {
		GLuint shader = glCreateShader(entry->type);

		entry->shader = shader;

		const GLchar* source = ReadShader(entry->filename);
		if (source == NULL) {
			for (entry = shaders; entry->type != GL_NONE; ++entry) {
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}

			return 0;
		}

		glShaderSource(shader, 1, &source, NULL);
		delete[] source;

		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
#ifdef _DEBUG
			GLsizei len;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetShaderInfoLog(shader, len, &len, log);
			std::cerr << "Shader compilation failed: " << log << std::endl;
			delete[] log;
#endif /* DEBUG */

			return 0;
		}

		glAttachShader(program, shader);

		++entry;
	}

	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
#ifdef _DEBUG
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		std::cerr << "Shader linking failed: " << log << std::endl;
		delete[] log;
#endif /* DEBUG */

		for (entry = shaders; entry->type != GL_NONE; ++entry) {
			glDeleteShader(entry->shader);
			entry->shader = 0;
		}

		return 0;
	}

	return program;
}

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

	isInited = true;

	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	GLfloat  vertices[6][2] = {
		{ -0.90f, -0.90f },{ 0.85f, -0.90f },{ -0.90f,  0.85f },  // Triangle 1
	{ 0.90f, -0.85f },{ 0.90f,  0.90f },{ -0.85f,  0.90f }   // Triangle 2
	};

	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER, "E://keypress.vert" },
		{ GL_FRAGMENT_SHADER, "E://keypress.frag" },
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