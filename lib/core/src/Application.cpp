#include "core/Application.h"

#include <cstdlib>
#include <iostream>

#include <SDL2/SDL.h>

namespace lib
{
namespace core
{

Application::Application(int argc, char * argv[])
{
    atexit(SDL_Quit);

    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        std::cerr << "SDL init failed:" << SDL_GetError() << std::endl;
        exit(-1);
    }
}

Application::~Application()
{
}

void Application::Run()
{
    mRunning = true;

    while(mRunning)
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                mRunning = false;
            else if(event.type == SDL_KEYUP)
            {
                if(SDLK_ESCAPE == event.key.keysym.sym)
                    mRunning = false;
            }
        }

        Update();

        SDL_Delay(16);
    }
}

} // namespace core
} // namespace lib
