#include "ShaderManager.h"


ShaderManager::ShaderManager()
{
	program = 0;
}

ShaderManager::~ShaderManager()
{
}

bool ShaderManager::loadShaders(const char * vertexFile, const char * fragmentFile)
{
	GLuint vShaderLoaded = loadShader(vertexFile, GL_VERTEX_SHADER);
	GLuint fShaderLoaded = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (!vShaderLoaded || !fShaderLoaded)
		return false;

	program = glCreateProgram();
	glAttachShader(program, vShaderLoaded);
	glAttachShader(program, fShaderLoaded);
	glLinkProgram(program);

	GLint progIv = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &progIv);
	if (progIv) return true;
	else
	{
		GLint logLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		GLchar* infoLog = new GLchar[logLength];
		glGetProgramInfoLog(program, logLength, 0, infoLog);

		std::cout << infoLog << std::endl;

		glDeleteProgram(program);
		delete[] infoLog;

		return false;
	}
}

GLuint ShaderManager::getProgram() const
{
	return program;
}

GLuint ShaderManager::loadShader(const char * file, GLenum shaderType)
{
	std::ifstream shaderStream;
	shaderStream.open(file, std::ios::binary);
	if (!shaderStream.is_open())
	{
		std::cout << "The file is not currently accessable." << std::endl;
		return 0;
	}
	else
	{
		//Seek to the end and get the length
		shaderStream.seekg(0, std::ios::end);
		uint32_t length = (uint32_t)shaderStream.tellg();
		//Seek back to the beginning
		shaderStream.seekg(0, std::ios::beg);
		//Make array to hold it all
		GLchar* shaderContent = new GLchar[length + 1];
		//Read it in and set the nullptr
		shaderStream.read(shaderContent, length);
		shaderContent[length] = 0;
		//Close the file
		shaderStream.close();

		GLuint shaderOut = glCreateShader(shaderType);
		glShaderSource(shaderOut, 1, &shaderContent, 0);
		glCompileShader(shaderOut);

		GLint shaderRun = 0;
		glGetShaderiv(shaderOut, GL_COMPILE_STATUS, &shaderRun);
		if (shaderRun != 0)
		{
			return (GLuint)shaderOut;
		}
		else
		{
			GLint logLength = 0;
			glGetShaderiv(shaderOut, GL_INFO_LOG_LENGTH, &logLength);

			GLchar* infoLog = new GLchar[logLength];
			glGetShaderInfoLog(shaderOut, logLength, 0, infoLog);
			
			std::cout << infoLog << std::endl;

			glDeleteShader(shaderOut);
			delete[] infoLog;

			return 0;
		}
	}
}
