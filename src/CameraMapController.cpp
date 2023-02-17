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

void CameraMapController::SetMapArea(const sgl::core::Pointd2D & t, const sgl::core::Pointd2D & r,
                                     const sgl::core::Pointd2D & b, const sgl::core::Pointd2D & l)
{
    mMapT = t;
    mMapR = r;
    mMapB = b;
    mMapL = l;
}

void CameraMapController::CenterCameraToPoint(int x, int y)
{
    const int hw = mCamera->GetWidth() / 2;
    const int hh = mCamera->GetHeight() / 2;
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
    const float halfP = 0.5f;

    // NOTE formula to check if center is inside the map area:
    // d = (x−x1)(y2−y1)−(y−y1)(x2−x1)
    sgl::core::Pointd2D cc(mCamera->GetX() + (mCamera->GetWidth() / 2),
                           mCamera->GetY() + (mCamera->GetHeight() / 2));

    bool inside = false;

    // HORIZONTAL
    const float movX = mDirX * mSpeed * delta;

    if(mDirX < 0)
    {
        cc.x = static_cast<int>(movX - halfP) + cc.x;

        if(cc.y < mMapL.y)
            inside = (cc.x - mMapT.x) * (mMapL.y - mMapT.y) > (cc.y - mMapT.y) * (mMapL.x - mMapT.x);
        else
            inside = (cc.x - mMapL.x) * (mMapB.y - mMapL.y) > (cc.y - mMapL.y) * (mMapB.x - mMapL.x);

        if(inside)
            mCamera->MoveX(movX);
    }
    else if(mDirX > 0)
    {
        cc.x = static_cast<int>(movX + halfP) + cc.x;

        if(cc.y < mMapL.y)
            inside = (cc.x - mMapR.x) * (mMapT.y - mMapR.y) > (cc.y - mMapR.y) * (mMapT.x - mMapR.x);
        else
            inside = (cc.x - mMapB.x) * (mMapR.y - mMapB.y) > (cc.y - mMapB.y) * (mMapR.x - mMapB.x);

        if(inside)
            mCamera->MoveX(movX);
    }

    // VERTICAL
    const float movY = mDirY * mSpeed * delta;

    if(mDirY < 0)
    {
        cc.y = static_cast<int>(movY - halfP) + cc.y;

        if(cc.x < mMapT.x)
            inside = (cc.x - mMapT.x) * (mMapL.y - mMapT.y) > (cc.y - mMapT.y) * (mMapL.x - mMapT.x);
        else
            inside = (cc.x - mMapR.x) * (mMapT.y - mMapR.y) > (cc.y - mMapR.y) * (mMapT.x - mMapR.x);

        if(inside)
            mCamera->MoveY(movY);
    }
    else if(mDirY > 0)
    {
        cc.y = static_cast<int>(movY + halfP) + cc.y;

        if(cc.x < mMapT.x)
            inside = (cc.x - mMapL.x) * (mMapB.y - mMapL.y) > (cc.y - mMapL.y) * (mMapB.x - mMapL.x);
        else
            inside = (cc.x - mMapB.x) * (mMapR.y - mMapB.y) > (cc.y - mMapB.y) * (mMapR.x - mMapB.x);

        if(inside)
           mCamera->MoveY(movY);
    }
}

} // namespace game
