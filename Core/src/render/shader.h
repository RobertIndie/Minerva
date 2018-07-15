#pragma once

#include <iostream>

#include <GL3/gl3w.h>
#include <GL3/gl3.h>
#include <GLFW/glfw3.h>

#include "../debug.h"

typedef struct {
	GLenum       type;
	const char*  filename;
	GLuint       shader;
} ShaderInfo;

class ShaderProgram
{
public:
	GLuint id;

	static ShaderProgram* LoadShaders(ShaderInfo*);
};

class ShaderProgram3D : public ShaderProgram
{
public:
	GLuint * VAO;
	GLuint* buffer;
	GLuint shaderProgram;

	float viewportAspect = 0.0f;
	GLint render_model_matrix_loc;
	GLint render_projection_matrix_loc;

};