#include "SDL_Input.h"

// Created by Joshua Cook
// For Graphics and Computational Programming

/// Input Constructor.
/// Initialises all keyboard inputs to false.
SDL_Input::SDL_Input(void)
{
	// initialise all keys to false
	for (int i = 0; i < 323; i++)
	{
		keysHeld[i] = false;
	}
	mousePress = mouseClick = false;

	mousePos = glm::vec2(0, 0);
}

SDL_Input::~SDL_Input(void)
{
}

/// Input Update.
/// Updates the keyBuffer array when called.
void SDL_Input::Update()
{
	// handle incoming events
	if (SDL_PollEvent(&incomingEvent))
	{
		// update the array of keys
		if (incomingEvent.type == SDL_KEYDOWN)
		{
			if (incomingEvent.key.keysym.sym < 323)
				keysHeld[incomingEvent.key.keysym.sym] = true;
		}
		if (incomingEvent.type == SDL_KEYUP)
		{
			if (incomingEvent.key.keysym.sym < 323)
				keysHeld[incomingEvent.key.keysym.sym] = false;
		}

		int tmpx, tmpy;
		SDL_GetMouseState(&tmpx, &tmpy);
		mousePos = glm::vec2(tmpx, tmpy);

		if (incomingEvent.type == SDL_MOUSEBUTTONDOWN)
		{
			if (!mousePress)
			{
				mousePress = true;
				mouseClick = true;
			}
			else
			{
				mouseClick = false;
			}
		}

		if (incomingEvent.type == SDL_MOUSEBUTTONUP)
		{
			mousePress = false;
			mouseClick = false;
		}
	}
}

/// Key Down.
/// returns the value of a passed in index for the keyBuffer array.
bool SDL_Input::isKeyDown(int key)
{
	if (key > 322)
		return false;

	return keysHeld[key];
}
/// Key Pressed.
/// returns the value of a passed in index for the keyBuffer array, and sets it to false as to indicate a press, rather than being held down.
bool SDL_Input::isKeyPressed(int key)
{
	if (key > 322)
		return false;

	bool ret = keysHeld[key];
	keysHeld[key] = false;

	return ret;
}

/// Escape Key.
/// A singled out function to see if the Escape key is pressed.
bool SDL_Input::Esc()
{
	return keysHeld[SDLK_ESCAPE];
}