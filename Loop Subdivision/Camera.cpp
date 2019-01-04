#include "Camera.h"

// Created by Joshua Cook
// For Graphics and Computational Programming

Camera::Camera(void)
{
	speed = 3.0f;
	mouseSpeed = 0.005f;

	pitch = 0;
	yaw = 0;

	fov = 45;
	position = glm::vec3(0, 0, 3);
	viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -2.5f));
	projectionMatrix = glm::perspective(70.0f, 16.0f / 9.0f, 0.1f, 100.0f);
}

Camera::~Camera(void)
{
}

void Camera::Orbit(glm::vec3 position, float distance)
{
	this->position = position;

	// Up

	// Right vector
	glm::vec3 right = glm::vec3(sin(yaw - 3.14f / 2.0f), 0, cos(yaw - 3.14f / 2.0f));

	// Up vector
	glm::vec3 up = glm::cross(right, Direction());

	this->position -= Direction() * distance;

	projectionMatrix = glm::perspective(fov, 16.0f / 9.0f, 0.1f, 100.0f);

	viewMatrix = glm::lookAt(this->position, this->position + Direction(), up);
}

glm::vec3 Camera::Direction()
{
	return glm::vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
}

void Camera::Update(SDL_Input* input, SDL_Window* window, float dt)
{
	//Restrain();
	// store the window size for later
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	width /= 2;
	height /= 2;

	// return the mouse to the center of the window
	yaw += mouseSpeed * (width - input->mousePosition().x);
	pitch += mouseSpeed * (height - input->mousePosition().y);

	SDL_WarpMouseInWindow(window, width, height);       //move back the cursor to the center of the screen

	// Right vector
	glm::vec3 right = glm::vec3(sin(yaw - 3.14f / 2.0f), 0, cos(yaw - 3.14f / 2.0f));

	// Up vector
	glm::vec3 up = glm::cross(right, Direction());

	// now we have the direction vectors, we can calulate movement
	// Move forward
	if (input->isKeyDown(SDLK_w))
	{
		position += Direction() * dt * speed;
	}
	// Move backward
	if (input->isKeyDown(SDLK_s))
	{
		position -= Direction() * dt * speed;
	}
	// Strafe right
	if (input->isKeyDown(SDLK_d))
	{
		position += right * dt * speed;
	}
	// Strafe left
	if (input->isKeyDown(SDLK_a))
	{
		position -= right * dt * speed;
	}
	// Move up
	if (input->isKeyDown(SDLK_SPACE))
	{
		position += up * dt * speed;
	}
	// Move down
	if (input->isKeyDown(SDLK_z))
	{
		position -= up * dt * speed;
	}
	projectionMatrix = glm::perspective(fov, 16.0f / 9.0f, 0.1f, 100.0f);

	viewMatrix = glm::lookAt(position, position + Direction(), up);
}

void Camera::Restrain()
{
	// keep the camera values within sensible numbers
	if (pitch > 30)
		pitch = 30;

	if (pitch < -30)
		pitch = -30;

	if (yaw < 0.0)
		yaw += 360.0;

	if (yaw > 360.0)
		yaw -= 360;
}