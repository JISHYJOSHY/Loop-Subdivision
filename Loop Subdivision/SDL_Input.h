#ifndef _INPUT_H_
#define _INPUT_H_

// Created by Joshua Cook
// For Graphics and Computational Programming

#include <SDL.h>
#include <glm.hpp>

class SDL_Input
{
public:
	SDL_Input(void);
	~SDL_Input(void);

	// update key status
	void Update();

	bool isKeyDown(int key);
	bool isKeyPressed(int key);

	// shortcut key for esc
	bool Esc();

	// return the state of the mouse
	inline bool mouseClicked() { return mouseClick; };
	inline bool mousePressed() { return mousePress; };

	// return the mouse position
	inline glm::vec2 mousePosition() { return mousePos; };

private:
	SDL_Event incomingEvent;
	bool keysHeld[323];

	bool mousePress, mouseClick;

	glm::vec2 mousePos;
};

#endif