#pragma once

namespace sgl
{
    namespace core
    {
        class KeyboardEvent;
        class MouseMotionEvent;
    }

    namespace graphic { class Camera; }
}

namespace game
{

class CameraMapController
{
public:
    CameraMapController(sgl::graphic::Camera * cam);

    const sgl::graphic::Camera * GetCamera() const;

    void SetLimits(int l, int r, int t, int b);

    void SetSpeed(float val);

    void CenterCameraToPoint(int x, int y);

    void HandleKeyDown(sgl::core::KeyboardEvent & event);
    void HandleKeyUp(sgl::core::KeyboardEvent & event);
    void HandleMouseMotion(sgl::core::MouseMotionEvent & event);

    void Update(float delta);

private:
    sgl::graphic::Camera * mCamera = nullptr;

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

inline const sgl::graphic::Camera * CameraMapController::GetCamera() const
{
    return mCamera;
}

inline void CameraMapController::SetLimits(int l, int r, int t, int b)
{
    mLimitL = l;
    mLimitR = r;
    mLimitT = t;
    mLimitB = b;
}

inline void CameraMapController::SetSpeed(float val) { mSpeed = val; }

} // namespace game
