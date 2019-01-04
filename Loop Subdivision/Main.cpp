#include "Application.h"
#include <time.h>

int main(int argc, char* argv[])
{
	Application app(1280, 720);
	srand(time(NULL));

	if (app.Init())
		app.Run();

	return 0;
}