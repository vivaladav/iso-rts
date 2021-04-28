#include "graphic/Camera.h"

namespace lib
{
namespace graphic
{

Camera * Camera::mDefault = nullptr;
Camera * Camera::mDummy = nullptr;

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
