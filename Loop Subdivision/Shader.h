#ifndef _SHADER_H_
#define _SHADER_H_

// Created by Joshua Cook
// For Graphics and Computational Programming

#include <string>
#include "glew.h"

class Shader
{
public:
	Shader(std::string vertexShader, std::string fragmentShader);
	~Shader(void);

	void LoadShader(std::string vertexShader, std::string fragmentShader);

	inline unsigned int getID() { return program; };

private:
	unsigned int program;

	std::string ReadFile(std::string filename);
};

#endif