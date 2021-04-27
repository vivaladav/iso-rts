#include "graphic/Camera.h"

namespace lib
{
namespace graphic
{

Camera * Camera::mDummy = nullptr;

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
