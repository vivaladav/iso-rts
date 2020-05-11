#include <iostream>

#include <graphic/Window.h>

#include <SDL2/SDL.h>

#include <cstdlib>

static const char * TITLE = "iso RTS - v. 0.0.1";

int main(int argc, char * argv[])
{
	std::cout << TITLE << std::endl;

	atexit(SDL_Quit);

	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		std::cerr << "SDL init failed:" << SDL_GetError() << std::endl;
		return -1;
	}

	lib::graphic::Window w(TITLE, 1600, 900);

	bool running = true;

	while(running)
	{
		SDL_Event event;

		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				running = false;
			else if(event.type == SDL_KEYUP)
			{
				if(SDLK_ESCAPE == event.key.keysym.sym)
					running = false;
			}
		}
	}


	return 0;
}
