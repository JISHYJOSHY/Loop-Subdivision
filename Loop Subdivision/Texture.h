#ifndef _TEXTURE_H_
#define _TEXTURE_H_

// Created by Joshua Cook
// For Graphics and Computational Programming

#include <string>
#include <SDL.h>
#include "glew.h"

class Texture
{
public:
	Texture(std::string filename, std::string textureType);
	~Texture(void);

	unsigned int ID;
	std::string type;

private:
	bool LoadTexture(std::string filename);
};

#endif