#include "Application.h"

#include <algorithm>

// Created by Joshua Cook
// For Graphics and Computational Programming

Application::Application(int WIDTH, int HEIGHT)
{
	winWidth = WIDTH;
	winHeight = HEIGHT;
}

Application::~Application(void)
{
}

void Application::LoadMesh(std::string filename)
{
	std::replace(filename.begin(), filename.end(), '\\', '/');
	std::cout << std::endl << filename;
	mesh.LoadMesh(filename);
}

// Initialisation of Application.
// Includes the startup of SDL and OpenGL.
bool Application::Init()
{
	// first init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Could not initialise SDL" << std::endl;
		return false;
	}

	system("CLS");
	std::cout << "Enter an ID(number) to load the following meshes: " << std::endl;
	std::cout << "1 : Cube\n2 : Hose\n3 : Pyramid\n4 : Torus\n5 : Torus Knot" << std::endl;

	std::string filename;
	int loadID;
	bool done = false;
	do
	{
		std::cin >> loadID;

		switch (loadID)
		{
		case 1:
			filename = "Models/Cube.obj";
			done = true;
			break;
		case 2:
			filename = "Models/Hose.obj";
			done = true;
			break;
		case 3:
			filename = "Models/Pyramid.obj";
			done = true;
			break;
		case 4:
			filename = "Models/Torus.obj";
			done = true;
			break;
		case 5:
			filename = "Models/TorusKnot.obj";
			done = true;
			break;
		default:
			std::cout << "Incorrect ID entered, please try again" << std::endl;
			break;
		}

	} while (!done);
	system("CLS");

	// then tell SDL we are using OpenGL	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// create a window
	window = SDL_CreateWindow("Joshua Cook Loop Subdivision", 300, 100, winWidth, winHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	// create the buffer
	renderer = SDL_CreateRenderer(window, -1, 0);

	// create OpenGL Context thing...
	glcontext = SDL_GL_CreateContext(window);

	// then load GLEW
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: GLEW failed to initialise with message: " << glewGetErrorString(err) << std::endl;
		return false;
	}

	// spit out GLEW + OpenGL info
	std::cout << "INFO: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

	std::cout << "INFO: OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "INFO: OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "INFO: OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// features of OpenGL we wish to use
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
	running = true;
	SetClearColour(0.4, 0.4, 0.8, 0.8);

	InitObjects();	

	mesh.LoadMesh(filename);

	system("CLS");
	std::cout << "Hello, press Space to Subdivide and hold the mouse button down and move the mouse to rotate the object\nUse the 'W' and 'S' keys to bring the object closer or further away" << std::endl;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPointSize(5);
	return true;
}

// Initialisation of Objects.
// Spserates the creation of the window and creation of GameObjects.
void Application::InitObjects()
{
	input = new SDL_Input();

	baseShader = new Shader("Shaders/basic.vert", "Shaders/basic.frag");

	mainCamera = new Camera();

	zoomAmount = 10;
}

// Frame Step of Application.
// Includes 'most' of the computation for the applications entities.
void Application::Update(float dt)
{
	if (input->isKeyPressed(SDLK_SPACE))
	{
		start = std::clock();
		mesh.Subdivide();
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		std::cout << "Elapsed time: " << duration << " seconds " << std::endl;
	}

	if (input->mousePressed())
		mainCamera->Update(input, window, dt);

	if (input->isKeyDown(SDLK_w))
		zoomAmount -= 2.f;
	if (input->isKeyDown(SDLK_s))
		zoomAmount += 2.f;

	mainCamera->Orbit(glm::vec3(0, 1, 0), zoomAmount);
}

// Draw Step of Application
// Includes the drawing of entities in the application
void Application::Draw()
{
	// Activate the shader program
	glUseProgram(baseShader->getID());

	glUniformMatrix4fv(glGetUniformLocation(baseShader->getID(), "viewMat"), 1, GL_FALSE, glm::value_ptr(mainCamera->getViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(baseShader->getID(), "projMat"), 1, GL_FALSE, glm::value_ptr(mainCamera->getProjectionMatrix()));

	mesh.Draw(baseShader);

	glUseProgram(0);
}

// Game Loop
// Not much to say here, constantly goes over Update and Draw functions
void Application::Run()
{
	unsigned int lastTime = SDL_GetTicks();

	while (running)
	{
		unsigned int current = SDL_GetTicks();
		float dt = (float)(current - lastTime) / 1000.0f;
		lastTime = current;

		if (dt < (1.0f / 60.0f))
		{
			SDL_Delay((unsigned int)(((1.0f / 60.0f) - dt) * 1000.0f));
		}

		input->Update();
		Update(dt);

		// reset buffer
		glClearColor(clearColour[0], clearColour[1], clearColour[2], clearColour[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Draw();

		// draw buffer
		SDL_GL_SwapWindow(window);

		if (input->Esc())
			running = false;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(glcontext);
}

// Set Frame Rate
// Caps the frame rate
void Application::SetTargetFrameRate(int target)
{
	targetFrameRate = target;
}

// Set Window Title
// Changes the window title
void Application::SetWindowTitle(std::string title)
{
	SDL_SetWindowTitle(window, title.c_str());
}

// Set Clear Colour
// Changes the OpenGL clear colour for the buffer
void Application::SetClearColour(float RED, float GREEN, float BLUE, float ALPHA)
{
	clearColour[0] = RED;
	clearColour[1] = GREEN;
	clearColour[2] = BLUE;
	clearColour[3] = ALPHA;
}