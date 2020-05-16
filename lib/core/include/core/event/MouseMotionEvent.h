#pragma once

#include "MouseEvent.h"

namespace lib
{
namespace core
{

class EventDispatcher;

class MouseMotionEvent : public MouseEvent
{
public:
    int GetX() const;
    int GetY() const;

    int GetDeltaX() const;
    int GetDeltaY() const;

    bool IsButtonPushed(int button) const;

private:
    MouseMotionEvent(int x, int y, int deltaX, int deltaY, int buttons);

private:
    int mX;
    int mY;
    int mDeltaX;
    int mDeltaY;

    int mButtonsState;

    // only EventDispatcher can create an event
    friend class EventDispatcher;
};

inline MouseMotionEvent::MouseMotionEvent(int x, int y, int deltaX, int deltaY, int buttons)
    : mX(x)
    , mY(y)
    , mDeltaX(deltaX)
    , mDeltaY(deltaY)
    , mButtonsState(buttons)
{
}

inline int MouseMotionEvent::GetX() const { return mX; }
inline int MouseMotionEvent::GetY() const { return mY; }

inline int MouseMotionEvent::GetDeltaX() const { return mDeltaX; }
inline int MouseMotionEvent::GetDeltaY() const { return mDeltaY; }

} // namespace core
} // namespace lib
