#include "graphic/Window.h"

#include <SDL2/SDL.h>

#define SYS_window static_cast<SDL_Window *>(mWin)

namespace lib
{
namespace graphic
{

Window::Window(const char * title, int w, int h)
{
	mWin = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_FOCUS);
}

Window::~Window()
{
	SDL_DestroyWindow(SYS_window);
}

} // namespace graphic
} // namespace lib
