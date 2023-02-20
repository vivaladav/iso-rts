#include "CameraMapController.h"

#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/core/event/MouseMotionEvent.h>
#include <sgl/graphic/Camera.h>

#include <limits>

#include <iostream>

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
    sgl::core::Pointd2D p(x, y);

    // if point not inside -> use orthogonal projection
    // LEFT
    if(x < mMapT.x)
    {
        // TOP - LEFT
        if(y < mMapL.y)
        {
            if(!IsPointInsideTL(p))
                p = GetVectorPojection(mMapL, mMapT, p);
        }
        // BOTTOM - LEFT
        else
        {
            // point not inside -> orthogonal projection
            if(!IsPointInsideBL(p))
                p = GetVectorPojection(mMapL, mMapB, p);
        }
    }
    // RIGHT
    else
    {
        // TOP - RIGHT
        if(y < mMapL.y)
        {
            if(!IsPointInsideTR(p))
                p = GetVectorPojection(mMapT, mMapR, p);
        }
        // BOTTOM - RIGHT
        else
        {
            if(!IsPointInsideTR(p))
                p = GetVectorPojection(mMapB, mMapR, p);
        }
    }

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

    // HORIZONTAL
    const float movX = mDirX * mSpeed * delta;

    if(mDirX < 0)
    {
        cc.x += static_cast<int>(movX - halfP);

        if((cc.y < mMapL.y && IsPointInsideTL(cc)) || (cc.y >= mMapL.y && IsPointInsideBL(cc)))
            mCamera->MoveX(movX);
    }
    else if(mDirX > 0)
    {
        cc.x += static_cast<int>(movX + halfP);

        if((cc.y < mMapL.y && IsPointInsideTR(cc)) || (cc.y >= mMapL.y && IsPointInsideBR(cc)))
            mCamera->MoveX(movX);
    }

    // VERTICAL
    const float movY = mDirY * mSpeed * delta;

    if(mDirY < 0)
    {
        cc.y += static_cast<int>(movY - halfP);

        if((cc.x < mMapT.x && IsPointInsideTL(cc)) || (cc.x >= mMapT.x && IsPointInsideTR(cc)))
            mCamera->MoveY(movY);
    }
    else if(mDirY > 0)
    {
        cc.y += static_cast<int>(movY + halfP);

        if((cc.x < mMapT.x && IsPointInsideBL(cc)) || (cc.x >= mMapT.x && IsPointInsideBR(cc)))
           mCamera->MoveY(movY);
    }
}

sgl::core::Pointd2D CameraMapController::GetVectorPojection(const sgl::core::Pointd2D & a0,
                                                            const sgl::core::Pointd2D & b0,
                                                            const sgl::core::Pointd2D & p) const
{
    const int ax = p.x - a0.x;
    const int ay = p.y - a0.y;
    const int bx = b0.x - a0.x;
    const int by = b0.y - a0.y;
    const float m = static_cast<float>(ax * bx + ay + by) / static_cast<float>(bx * bx + by * by);

    const sgl::core::Pointd2D ret((bx * m) + a0.x, (by * m) + a0.y);
    return ret;
}

} // namespace game
