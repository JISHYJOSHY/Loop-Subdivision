#include "Shader.h"
#include <iostream>
#include <fstream>

// Created by Joshua Cook
// For Graphics and Computational Programming 

Shader::Shader(std::string vertexShader, std::string fragmentShader)
{
	LoadShader(vertexShader, fragmentShader);
}


Shader::~Shader(void)
{
}

/// Check to see if the shader has compiled
bool CheckShaderCompiled(GLint shader)
{
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLsizei len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		// OpenGL will store an error message as a string that we can retrieve and print
		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(shader, len, &len, log);
		std::cerr << "ERROR: Shader compilation failed: " << log << std::endl;
		delete[] log;

		return false;
	}
	return true;
}

void Shader::LoadShader(std::string vertShader, std::string fragShader)
{
	// This is the vertex shader being loaded from file
	std::string VertexShaderCode = ReadFile(vertShader);

	// This is the fragment shader
	std::string FragShaderCode = ReadFile(fragShader);

	// The 'program' stores the shaders
	program = glCreateProgram();

	// Create the vertex shader
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	// Give GL the source for it
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(vShader, 1, &VertexSourcePointer, NULL);
	// Compile the shader
	glCompileShader(vShader);
	// Check it compiled and give useful output if it didn't work!
	if (!CheckShaderCompiled(vShader))
	{
		return;
	}
	// This links the shader to the program
	glAttachShader(program, vShader);

	// Same for the fragment shader
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	char const* FragSourcePointer = FragShaderCode.c_str();
	glShaderSource(fShader, 1, &FragSourcePointer, NULL);
	glCompileShader(fShader);
	if (!CheckShaderCompiled(fShader))
	{
		return;
	}
	glAttachShader(program, fShader);

	// This makes sure the vertex and fragment shaders connect together
	glLinkProgram(program);
	// Check this worked
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
		delete[] log;

		return;
	}

	// Here, we set up the inputs for the shader
}

std::string Shader::ReadFile(std::string fileName) {

	std::string result;

	std::ifstream stream(fileName, std::ios::in);
	if (stream.is_open()){
		std::string Line = "";
		while (getline(stream, Line))
			result += "\n" + Line;
		stream.close();
	}

	return result;
}