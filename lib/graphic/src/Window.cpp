#include "graphic/Window.h"

#include <SDL2/SDL.h>

namespace lib
{
namespace graphic
{

Window * Window::mInstance = nullptr;

Window * Window::Create(const char * title, int w, int h)
{
    if(!mInstance)
        mInstance = new Window(title, w, h);

    return mInstance;
}

void Window::Destroy()
{
    delete mInstance;
    mInstance = nullptr;
}

void Window::SetSize(int w, int h)
{
    mW = w;
    mH = h;

    SDL_SetWindowSize(mSysWin, w, h);
}

void Window::SetFullscreen(bool f)
{
    if(mFullscreen == f)
        return ;

    mFullscreen = f;

    const unsigned int flag = mFullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    SDL_SetWindowFullscreen(mSysWin, flag);
}

Window::Window(const char * title, int w, int h)
    : mW(w)
    , mH(h)
{
    const int posX = SDL_WINDOWPOS_CENTERED;
    const int posY = SDL_WINDOWPOS_CENTERED;
    const int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_FOCUS;

    mSysWin = SDL_CreateWindow(title, posX, posY, w, h, flags);
}

Window::~Window()
{
    SDL_DestroyWindow(mSysWin);
}

} // namespace graphic
} // namespace lib
