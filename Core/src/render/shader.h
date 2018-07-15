#pragma once

#include <iostream>

#include <GL3/gl3w.h>
#include <GL3/gl3.h>
#include <GLFW/glfw3.h>

#include "../debug.h"
#include "../util/util.h"

typedef struct {
	GLenum       type;
	const char*  filename;
	GLuint       shader;
} ShaderInfo;

class ShaderProgram
{
private:
	bool loaded = false;
	bool inited = false;
public:
	GLuint id;
	GLuint* VAO;
	GLuint* buffer;
	virtual void Initialize();
	void LoadShaders(ShaderInfo*);
	~ShaderProgram();
};

class ShaderProgram3D : public ShaderProgram
{
public:
	typedef class ShaderProgram base;
	GLint render_model_matrix_loc;
	GLint render_projection_matrix_loc;
	void Initialize();
	void SetModelMatrix(vmath::mat4);
	void SetProjectionMatrix(vmath::mat4);
};