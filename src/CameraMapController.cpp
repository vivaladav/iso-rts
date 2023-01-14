#include "CameraMapController.h"

#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/core/event/MouseMotionEvent.h>
#include <sgl/graphic/Camera.h>

#include <limits>

namespace game
{

constexpr float DEF_SPEED = 400.f;
constexpr int SCROLL_L = -1;
constexpr int SCROLL_R = 1;
constexpr int SCROLL_U = -1;
constexpr int SCROLL_D = 1;
constexpr int NO_SCROLL = 0;

CameraMapController::CameraMapController(sgl::graphic::Camera * cam)
    : mCamera(cam)
    , mSpeed(DEF_SPEED)
    , mDirX(NO_SCROLL)
    , mDirY(NO_SCROLL)
    , mLimitL(std::numeric_limits<int>::min())
    , mLimitR(std::numeric_limits<int>::max())
    , mLimitT(std::numeric_limits<int>::min())
    , mLimitB(std::numeric_limits<int>::max())
{
}

void CameraMapController::CenterCameraToPoint(int x, int y)
{
    const int hw = mCamera->GetWidth() * 0.5f;
    const int hh = mCamera->GetHeight() * 0.5f;
    const int cameraX0 = x - hw;
    const int cameraY0 = y - hh;

    // clamp X
    if(cameraX0 < mLimitL)
        x = mLimitL + hw;
    else if(cameraX0 > mLimitR)
        x = mLimitR + hw;

    // clamp Y
    if(cameraY0 < mLimitT)
        y = mLimitT + hh;
    else if(cameraY0 > mLimitB)
        y = mLimitB + hh;

    mCamera->CenterToPoint(x, y);
}

void CameraMapController::HandleKeyDown(sgl::core::KeyboardEvent & event)
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

void CameraMapController::HandleKeyUp(sgl::core::KeyboardEvent & event)
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

void CameraMapController::HandleMouseMotion(sgl::core::MouseMotionEvent & event)
{
    const int screenX = event.GetX();
    const int screenY = event.GetY();

    const int scrollingMargin = 10;

    if(screenX < scrollingMargin)
    {
        if(!mKeyScrollX)
        {
            mDirX = SCROLL_L;
            mMouseScrollX = true;
        }
    }
    else if(screenX > (mCamera->GetWidth() - scrollingMargin))
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
    else if(screenY > (mCamera->GetHeight() - scrollingMargin))
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

void CameraMapController::HandleMouseLeftWindow()
{
    mMouseScrollX = false;
    mMouseScrollY = false;

    if(!mKeyScrollX)
        mDirX = NO_SCROLL;

    if(!mKeyScrollY)
        mDirY = NO_SCROLL;
}

void CameraMapController::Update(float delta)
{
    // HORIZONTAL
    const float movX = mDirX * mSpeed * delta;

    if(mDirX < 0)
    {
        const int newX = static_cast<int>(movX - 0.5f) + mCamera->GetX();

        if(newX < mLimitL)
            mCamera->SetX(mLimitL);
        else
            mCamera->MoveX(movX);
    }
    else if(mDirX > 0)
    {
        const int newX = static_cast<int>(movX + 0.5f) + mCamera->GetX();

        if(newX > mLimitR)
            mCamera->SetX(mLimitR);
        else
            mCamera->MoveX(movX);
    }

    // VERTICAL
    const float movY = mDirY * mSpeed * delta;

    if(mDirY < 0)
    {
        const int newY = static_cast<int>(movY - 0.5f) + mCamera->GetY();

        if(newY < mLimitT)
            mCamera->SetY(mLimitT);
        else
            mCamera->MoveY(movY);
    }
    else if(mDirY > 0)
    {
        const int newY = static_cast<int>(movY + 0.5f) + mCamera->GetY();

        if(newY > mLimitB)
            mCamera->SetY(mLimitB);
        else
            mCamera->MoveY(movY);
    }
}

} // namespace game
