#pragma once

#include <sgl/graphic/Camera.h>

namespace sgl
{
    namespace core
    {
        class KeyboardEvent;
        class MouseMotionEvent;
    }
}

namespace game
{

class CameraMap : public sgl::graphic::Camera
{
public:
    CameraMap(int viewW, int viewH);

    void SetLimits(int l, int r, int t, int b);

    void SetSpeed(float val);

    void HandleKeyDown(sgl::core::KeyboardEvent & event);
    void HandleKeyUp(sgl::core::KeyboardEvent & event);
    void HandleMouseMotion(sgl::core::MouseMotionEvent & event);

    void Update(float delta);

private:
    float mSpeed;
    int mDirX;
    int mDirY;
    int mLimitL;
    int mLimitR;
    int mLimitT;
    int mLimitB;
    bool mKeyScrollX = false;
    bool mKeyScrollY = false;
    bool mMouseScrollX = false;
    bool mMouseScrollY = false;
};

inline void CameraMap::SetLimits(int l, int r, int t, int b)
{
    mLimitL = l;
    mLimitR = r;
    mLimitT = t;
    mLimitB = b;
}

inline void CameraMap::SetSpeed(float val) { mSpeed = val; }

} // namespace game
