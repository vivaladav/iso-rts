#pragma once

#include "MouseEvent.h"

namespace lib
{
namespace core
{

class EventDispatcher;

class MouseButtonEvent : public MouseEvent
{
public:
    int GetX() const;
    int GetY() const;

    int GetButton() const;

private:
    MouseButtonEvent(int x, int y, int button);

private:
    int mX;
    int mY;

    int mButton;

    // only EventDispatcher can create an event
    friend class EventDispatcher;
};

inline MouseButtonEvent::MouseButtonEvent(int x, int y, int button)
    : mX(x)
    , mY(y)
    , mButton(button)
{
}

inline int MouseButtonEvent::GetX() const { return mX; }
inline int MouseButtonEvent::GetY() const { return mY; }

inline int MouseButtonEvent::GetButton() const { return mButton; }

} // namespace core
} // namespace lib
