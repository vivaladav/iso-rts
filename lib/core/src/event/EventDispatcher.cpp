#include "EventDispatcher.h"

#include "core/event/EventListener.h"
#include "core/event/KeyboardEvent.h"
#include "core/event/MouseButtonEvent.h"
#include "core/event/MouseMotionEvent.h"

#include <SDL2/SDL.h>

#include <cassert>

namespace lib
{
namespace core
{

EventDispatcher::EventDispatcher(EventListener * listener) : mActiveListener(listener)
{
    assert(listener);
}

void EventDispatcher::SetListener(EventListener * listener)
{
    assert(listener);

    mActiveListener = listener;
}

void EventDispatcher::Update()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
            {
                MouseMotionEvent e(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel, event.motion.state);
                mActiveListener->OnMouseMotion(e);
            }
            break;

            case SDL_MOUSEBUTTONDOWN:
            {
                MouseButtonEvent e(event.button.x, event.button.y, event.button.button);
                mActiveListener->OnMouseButtonDown(e);
            }
            break;

            case SDL_MOUSEBUTTONUP:
            {
                MouseButtonEvent e(event.button.x, event.button.y, event.button.button);
                mActiveListener->OnMouseButtonUp(e);
            }
            break;

            case SDL_KEYDOWN:
            {
                KeyboardEvent e(event.key.keysym.sym);
                mActiveListener->OnKeyDown(e);
            }
            break;

            case SDL_KEYUP:
            {
                KeyboardEvent e(event.key.keysym.sym);
                mActiveListener->OnKeyUp(e);
            }
            break;

            case SDL_QUIT:
                mActiveListener->OnApplicationQuit();
            break;

            default:
            break;
        }
    }
}

} // namespace core
} // namespace lib

