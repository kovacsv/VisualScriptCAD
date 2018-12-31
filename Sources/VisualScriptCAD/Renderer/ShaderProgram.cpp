#include "ShaderProgram.hpp"
#include <glad/glad.h>

static int CompileShader (GLenum type, const char* shaderSource)
{
	int shaderId = glCreateShader (type);
	glShaderSource (shaderId, 1, &shaderSource, NULL);
	glCompileShader (shaderId);

	int success = 0;
	glGetShaderiv (shaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[1024];
		glGetShaderInfoLog (shaderId, 1024, NULL, infoLog);
		return -1;
	}

	return shaderId;
}

static int LinkShaders (int vertexShader, int fragmentShader)
{
	int shaderProgram = glCreateProgram ();
	glAttachShader (shaderProgram, vertexShader);
	glAttachShader (shaderProgram, fragmentShader);
	glLinkProgram (shaderProgram);
	glDeleteShader (vertexShader);
	glDeleteShader (fragmentShader);

	int success = 0;
	glGetProgramiv (shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[1024];
		glGetProgramInfoLog (shaderProgram, 1024, NULL, infoLog);
		return -1;
	}

	return shaderProgram;
}

int CreateShaderProgram (const char* vertexShaderSource, const char* fragmentShaderSource)
{
	int vertexShader = CompileShader (GL_VERTEX_SHADER, vertexShaderSource);
	int fragmentShader = CompileShader (GL_FRAGMENT_SHADER, fragmentShaderSource);
	if (vertexShader < 0 || fragmentShader < 0) {
		return -1;
	}

	int shaderProgram = LinkShaders (vertexShader, fragmentShader);
	if (shaderProgram < 0) {
		return -1;
	}

	return shaderProgram;
}
