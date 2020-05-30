#include "EventDispatcher.h"

#include "core/event/EventListener.h"
#include "core/event/KeyboardEvent.h"
#include "core/event/MouseButtonEvent.h"
#include "core/event/MouseMotionEvent.h"

#include <SDL2/SDL.h>

#include <algorithm>
#include <cassert>

namespace lib
{
namespace core
{

void EventDispatcher::AddListener(lib::core::EventListener * el)
{
    // do not add NULL
    if(!el)
        return ;

    auto it = std::find(mListeners.begin(), mListeners.end(), el);

    // listener not found -> add it
    if(mListeners.end() == it)
        mListeners.emplace_back(el);
}

void EventDispatcher::RemoveListener(EventListener * el)
{
    auto it = std::find(mListeners.begin(), mListeners.end(), el);

    // listener found -> remove it
    if(it != mListeners.end())
        mListeners.erase(it);
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

                for(EventListener * el : mListeners)
                    el->OnMouseMotion(e);
            }
            break;

            case SDL_MOUSEBUTTONDOWN:
            {
                MouseButtonEvent e(event.button.x, event.button.y, event.button.button);

                for(EventListener * el : mListeners)
                    el->OnMouseButtonDown(e);
            }
            break;

            case SDL_MOUSEBUTTONUP:
            {
                MouseButtonEvent e(event.button.x, event.button.y, event.button.button);

                for(EventListener * el : mListeners)
                    el->OnMouseButtonUp(e);
            }
            break;

            case SDL_KEYDOWN:
            {
                KeyboardEvent e(event.key.keysym.sym);

                for(EventListener * el : mListeners)
                    el->OnKeyDown(e);
            }
            break;

            case SDL_KEYUP:
            {
                KeyboardEvent e(event.key.keysym.sym);

                for(EventListener * el : mListeners)
                    el->OnKeyUp(e);
            }
            break;

            case SDL_QUIT:
                for(EventListener * el : mListeners)
                    el->OnApplicationQuit();
            break;

            default:
            break;
        }
    }
}

} // namespace core
} // namespace lib

