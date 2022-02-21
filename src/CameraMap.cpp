#include "CameraMap.h"

#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/core/event/MouseMotionEvent.h>

#include <limits>

namespace game
{

constexpr float DEF_SPEED = 400.f;
constexpr int SCROLL_L = -1;
constexpr int SCROLL_R = 1;
constexpr int SCROLL_U = -1;
constexpr int SCROLL_D = 1;
constexpr int NO_SCROLL = 0;

CameraMap::CameraMap(int viewW, int viewH)
    : sgl::graphic::Camera(viewW, viewH)
    , mSpeed(DEF_SPEED)
    , mDirX(NO_SCROLL)
    , mDirY(NO_SCROLL)
    , mLimitL(std::numeric_limits<int>::min())
    , mLimitR(std::numeric_limits<int>::max())
    , mLimitT(std::numeric_limits<int>::min())
    , mLimitB(std::numeric_limits<int>::max())
{
}

void CameraMap::HandleKeyDown(sgl::core::KeyboardEvent & event)
{
    using namespace sgl::core;

    const int key = event.GetKey();

    if(key == KeyboardEvent::KEY_LEFT)
    {
        if(!mMouseScrollX)
        {
            mDirX = SCROLL_L;
            mKeyScrollX = true;
        }
    }
    else if(key == KeyboardEvent::KEY_RIGHT)
    {
        if(!mMouseScrollX)
        {
            mDirX = SCROLL_R;
            mKeyScrollX = true;
        }
    }
    else if(key == KeyboardEvent::KEY_UP)
    {
        if(!mMouseScrollY)
        {
            mDirY = SCROLL_U;
            mKeyScrollY = true;
        }
    }
    else if(key == KeyboardEvent::KEY_DOWN)
    {
        if(!mMouseScrollY)
        {
            mDirY = SCROLL_D;
            mKeyScrollY = true;
        }
    }
}

void CameraMap::HandleKeyUp(sgl::core::KeyboardEvent & event)
{
    using namespace sgl::core;

    const int key = event.GetKey();

    if(key == KeyboardEvent::KEY_LEFT)
    {
        if(!mMouseScrollX)
            mDirX = NO_SCROLL;

        mKeyScrollX = false;
    }
    else if(key == KeyboardEvent::KEY_RIGHT)
    {
        if(!mMouseScrollX)
            mDirX = NO_SCROLL;

        mKeyScrollX = false;
    }
    else if(key == KeyboardEvent::KEY_UP)
    {
        if(!mMouseScrollY)
            mDirY = NO_SCROLL;

        mKeyScrollY = false;
    }
    else if(key == KeyboardEvent::KEY_DOWN)
    {
        if(!mMouseScrollY)
            mDirY = NO_SCROLL;

        mKeyScrollY = false;
    }
}

void CameraMap::HandleMouseMotion(sgl::core::MouseMotionEvent & event)
{
    const int screenX = event.GetX();
    const int screenY = event.GetY();

    const int scrollingMargin = 5;

    if(screenX < scrollingMargin)
    {
        if(!mKeyScrollX)
        {
            mDirX = SCROLL_L;
            mMouseScrollX = true;
        }
    }
    else if(screenX > (GetWidth() - scrollingMargin))
    {
        if(!mKeyScrollX)
        {
            mDirX = SCROLL_R;
            mMouseScrollX = true;
        }
    }
    else if(!mKeyScrollX)
    {
        mDirX = NO_SCROLL;
        mMouseScrollX = false;
    }

    if(screenY < scrollingMargin)
    {
        if(!mKeyScrollY)
        {
            mDirY = SCROLL_U;
            mMouseScrollY = true;
        }
    }
    else if(screenY > (GetHeight() - scrollingMargin))
    {
        if(!mKeyScrollY)
        {
            mDirY = SCROLL_D;
            mMouseScrollY = true;
        }
    }
    else if(!mKeyScrollY)
    {
        mDirY = NO_SCROLL;
        mMouseScrollY = false;
    }
}

void CameraMap::Update(float delta)
{
    // HORIZONTAL
    const float movX = mDirX * mSpeed * delta;

    if(mDirX < 0)
    {
        const int newX = static_cast<int>(movX - 0.5f) + GetX();

        if(newX < mLimitL)
            SetX(mLimitL);
        else
            MoveX(movX);
    }
    else if(mDirX > 0)
    {
        const int newX = static_cast<int>(movX + 0.5f) + GetX();

        if(newX > mLimitR)
            SetX(mLimitR);
        else
            MoveX(movX);
    }

    // VERTICAL
    const float movY = mDirY * mSpeed * delta;

    if(mDirY < 0)
    {
        const int newY = static_cast<int>(movY - 0.5f) + GetY();

        if(newY < mLimitT)
            SetY(mLimitT);
        else
            MoveY(movY);
    }
    else if(mDirY > 0)
    {
        const int newY = static_cast<int>(movY + 0.5f) + GetY();

        if(newY > mLimitB)
            SetY(mLimitB);
        else
            MoveY(movY);
    }
}

} // namespace game
