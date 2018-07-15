#include "shader.h"

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
		message err;
		err << "Unable to open file '" << filename << "'" << std::endl;
		errorInput(M2CSTR(err));
#endif /* DEBUG */
		return NULL;
	}

	// To get len.
	fseek(infile, 0, SEEK_END);//seek endo position
	int len = ftell(infile);//return the length of file
	fseek(infile, 0, SEEK_SET);//seek back to start position

	GLchar* source = new GLchar[len + 1];

	fread(source, 1, len, infile);
	fclose(infile);

	source[len] = 0;

	return const_cast<const GLchar*>(source);
}

GLuint
internal_LoadShaders(ShaderInfo* shaders)
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
			message err;
			err << "Shader compilation failed: " << log << std::endl;
			errorInput(M2CSTR(err));
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
		message err;
		err << "Shader linking failed: " << log << std::endl;
		errorInput(M2CSTR(err));
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

void
ShaderProgram::Initialize()
{
	VAO = new GLuint;
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	buffer = new GLuint;
	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);

	inited = true;
}

void
ShaderProgram::LoadShaders(ShaderInfo* shaders)
{
	GLuint program = internal_LoadShaders(shaders);
	id = program;
	loaded = true;
}

ShaderProgram::~ShaderProgram()
{
	glUseProgram(0);
	glDeleteProgram(id);
	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(1, buffer);
	delete VAO;
	VAO = NULL;
	delete buffer;
	buffer = NULL;
}

void
ShaderProgram3D::Initialize()
{
	base::Initialize();
	render_model_matrix_loc = glGetUniformLocation(id, "model_matrix");
	render_projection_matrix_loc = glGetUniformLocation(id, "projection_matrix");
}

void
ShaderProgram3D::SetModelMatrix(vmath::mat4 modelMatrix)
{
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, modelMatrix);
}

void 
ShaderProgram3D::SetProjectionMatrix(vmath::mat4 projectionMatrix)
{
	glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projectionMatrix);
}