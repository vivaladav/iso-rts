#include "graphic/Camera.h"

#include <cmath>

namespace lib
{
namespace graphic
{

Camera * Camera::mDefault = nullptr;
Camera * Camera::mDummy = nullptr;

Camera::Camera()
    : mFuncOnMove([]{})
{
}

Camera::Camera(int w, int h)
    : mFuncOnMove([]{})
    , mWidth(w)
    , mHeight(h)
{
}

void Camera::MoveX(float delta)
{
    mXf += delta;

    mXd = std::roundf(mXf);

    mFuncOnMove();
}

void Camera::MoveY(float delta)
{
    mYf += delta;

    mYd = std::roundf(mYf);

    mFuncOnMove();
}

void Camera::ResetPosition()
{
    mXf = 0.f;
    mYf = 0.f;

    mXd = 0;
    mYd = 0;

    mFuncOnMove();
}

void Camera::CenterToPoint(int x, int y)
{
    mXf = x - (mWidth * 0.5f);
    mYf = y - (mHeight * 0.5f);

    mXd = std::roundf(mXf);
    mYd = std::roundf(mYf);

    mFuncOnMove();
}

void Camera::CreateDefaultCamera()
{
    if(nullptr == mDefault)
        mDefault = new Camera;
}

void Camera::DestroyDefaultCamera()
{
    delete mDefault;
    mDefault = nullptr;
}

void Camera::CreateDummyCamera()
{
    if(nullptr == mDummy)
        mDummy = new Camera;
}

void Camera::DestroyDummyCamera()
{
    delete mDummy;
    mDummy = nullptr;
}

} // namespace graphic
} // namespace lib
