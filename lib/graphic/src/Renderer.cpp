#include "graphic/Renderer.h"

#include "graphic/Window.h"

#include <SDL2/SDL.h>

#include <cassert>

#define SYS_render static_cast<SDL_Renderer *>(mRenderer)

namespace lib
{
namespace graphic
{


Renderer::Renderer(Window * win)
{
	assert(win);

	mRenderer = SDL_CreateRenderer(static_cast<SDL_Window *>(win->mWin), -1, SDL_RENDERER_ACCELERATED);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(SYS_render);
}

void Renderer::Clear()
{
	SDL_RenderClear(SYS_render);
}

void Renderer::Finalize()
{
	SDL_RenderPresent(SYS_render);
}

} // namespace graphic
} // namespace lib
