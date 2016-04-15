#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <fstream>
#include <iostream>

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	bool loadShaders(const char* vertexFile, const char* fragmentFile);
	GLuint getProgram() const;

private:
	GLuint loadShader(const char* file, GLenum shaderType);
	GLuint program;
};

