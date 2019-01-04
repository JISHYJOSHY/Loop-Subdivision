#ifndef _CAMERA_H_
#define _CAMERA_H_

// Created by Joshua Cook
// For Graphics and Computational Programming

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "SDL_Input.h"

class Camera
{
public:
	Camera(void);
	~Camera(void);

	inline glm::vec3 getPosition() { return position; };
	inline glm::mat4 getViewMatrix() { return viewMatrix; };
	inline glm::mat4 getProjectionMatrix() { return projectionMatrix; };
	void Update(SDL_Input* input, SDL_Window* window, float dt);

	void Orbit(glm::vec3 position, float distance);

	inline void setFOV(int i) { fov = i; };
	inline void setPosition(glm::vec3 pos) { position = pos; };
	inline void setPosition(float x, float y, float z) { position = glm::vec3(x, y, z); };

	glm::vec3 Direction();

private:
	glm::vec3 position;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::vec2 mousePosOld;

	float speed;
	float mouseSpeed;

	float pitch, yaw;

	float fov;

	void Restrain();
};

#endif