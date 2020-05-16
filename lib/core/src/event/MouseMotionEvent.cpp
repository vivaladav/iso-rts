#include "core/event/MouseMotionEvent.h"

#include <SDL2/SDL.h>

namespace lib
{
namespace core
{

bool MouseMotionEvent::IsButtonPushed(int button) const
{
    if(BUTTON_LEFT == button)
        return SDL_BUTTON_LMASK & mButtonsState;
    else if(BUTTON_RIGHT == button)
        return SDL_BUTTON_RMASK & mButtonsState;
    else if(BUTTON_MIDDLE == button)
        return SDL_BUTTON_MMASK & mButtonsState;
    else if(BUTTON_X1 == button)
        return SDL_BUTTON_X1MASK & mButtonsState;
    else if(BUTTON_X2 == button)
        return SDL_BUTTON_X2MASK & mButtonsState;
    else
        return false;
}

} // namespace core
} // namespace lib
