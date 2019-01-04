#include "Texture.h"
#include <iostream>

// Created by Joshua Cook
// For Graphics and Computational Programming

Texture::Texture(std::string filename, std::string textureType)
{
	type = textureType;
	LoadTexture(filename);
}

Texture::~Texture(void)
{
}

bool Texture::LoadTexture(std::string filename)
{
	// load bmp through SDL
	SDL_Surface* image = SDL_LoadBMP(filename.c_str());

	// check if it has loaded
	if (!image)
	{
		std::cout << "FAILED : Cannot open file: '" << filename << "'. " << std::endl;
		return false;
	}

	// use the SDL imported data to generate a texture through glew
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// BMP files are loaded in with the colour channels backwards, so we need to process it as a BGR instead of RGB
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image->pixels);

	// OpenGL now has our image data, so we can free the loaded image
	SDL_FreeSurface(image);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	return true;
}