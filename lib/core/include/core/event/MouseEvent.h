#pragma once

#include "Event.h"

namespace lib
{
namespace core
{

class MouseEvent : public Event
{
public:
    static const int BUTTON_LEFT;
    static const int BUTTON_MIDDLE;
    static const int BUTTON_RIGHT;
    static const int BUTTON_X1;
    static const int BUTTON_X2;

protected:
    MouseEvent();
};

inline MouseEvent::MouseEvent() {}

} // namespace core
} // namespace lib
