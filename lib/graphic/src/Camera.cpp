#include "graphic/Camera.h"

#include <cmath>

namespace lib
{
namespace graphic
{

Camera * Camera::mDefault = nullptr;
Camera * Camera::mDummy = nullptr;


void Camera::MoveX(float delta)
{
    mXf += delta;

    mXd = std::roundf(mXf);
}

void Camera::MoveY(float delta)
{
    mYf += delta;

    mYd = std::roundf(mYf);
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
