#ifndef _APPLICATION_H_
#define _APPLICATION_H_

// Created by Joshua Cook
// For Graphics and Computational Programming

#include <SDL.h>
#include <iostream>
#include "glew.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <ctime>

#include "SDL_Input.h"
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"

class Application
{
public:
	Application(int WIDTH, int HEIGHT);
	~Application(void);

	void LoadMesh(std::string filename);

	bool Init();
	void Run();

protected:
	int winWidth;
	int winHeight;

	virtual void InitObjects();

	virtual void Update(float dt);
	virtual void Draw();

	void SetTargetFrameRate(int target);
	void SetWindowTitle(std::string title);
	void SetClearColour(float RED, float GREEN, float BLUE, float ALPHA);

	SDL_Input* input;
	Camera* mainCamera;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_GLContext glcontext;

	Shader* baseShader;
	Mesh mesh;

private:
	bool running;
	int targetFrameRate;

	float clearColour[4];
	float zoomAmount;

	std::clock_t start;
	double duration;
};

#endif