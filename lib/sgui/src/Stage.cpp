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

void Stage::OnMouseButtonDown(core::MouseButtonEvent & event)
{
    PropagateMouseButtonDown(event);
}

void Stage::OnMouseButtonUp(core::MouseButtonEvent & event)
{
    PropagateMouseButtonUp(event);
}

void Stage::OnMouseMotion(core::MouseMotionEvent & event)
{
    PropagateMouseMotion(event);
}

void Stage::OnKeyDown(core::KeyboardEvent & event)
{
    PropagateKeyDown(event);
}

void Stage::OnKeyUp(core::KeyboardEvent & event)
{
    PropagateKeyUp(event);
}

} // namespace sgui
} // namespace lib
