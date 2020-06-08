#include "sgui/Stage.h"

#include "core/event/MouseButtonEvent.h"
#include "core/event/MouseMotionEvent.h"
#include "sgui/Widget.h"

#include <algorithm>

namespace lib
{
namespace sgui
{

Stage * Stage::mInstance = nullptr;

Stage * Stage::Create()
{
    if(!mInstance)
        mInstance = new Stage;

    return mInstance;
}

void Stage::Destroy()
{
    delete mInstance;
    mInstance = nullptr;
}

void Stage::Render()
{
    PropagateRender();
}

void Stage::OnMouseButtonDown(const core::MouseButtonEvent & event)
{
    PropagateMouseButtonDown(event);
}

void Stage::OnMouseButtonUp(const core::MouseButtonEvent & event)
{
    PropagateMouseButtonUp(event);
}

void Stage::OnMouseMotion(const core::MouseMotionEvent & event)
{
    PropagateMouseMotion(event);
}

} // namespace sgui
} // namespace lib
