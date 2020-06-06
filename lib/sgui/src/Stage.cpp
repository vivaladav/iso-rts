#include "sgui/Stage.h"

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
    for(Widget * w : mWidgets)
    {
        w->OnRender();
        w->PropagateRender();
    }
}

Stage::Stage() { }

Stage::~Stage()
{
    for(Widget * w : mWidgets)
        delete w;
}

void Stage::AddChild(Widget * w)
{
    // widget already added
    if(std::find(mWidgets.begin(), mWidgets.end(), w) != mWidgets.end())
        return ;

    mWidgets.emplace_back(w);
}

void Stage::OnChildEnableChanged(Widget * /*child*/)
{
    // TODO - handle enable changed in child
}

void Stage::OnChildVisibleChanged(Widget * /*child*/)
{
    // TODO - handle visible changed in child
}

void Stage::OnMouseButtonDown(const core::MouseButtonEvent & event)
{
    for(Widget * w : mWidgets)
    {
        w->PropagateMouseButtonDown(event);
        w->HandleMouseButtonDown(event);
    }
}

void Stage::OnMouseButtonUp(const core::MouseButtonEvent & event)
{
    for(Widget * w : mWidgets)
    {
        w->PropagateMouseButtonUp(event);
        w->HandleMouseButtonUp(event);
    }
}

void Stage::OnMouseMotion(const core::MouseMotionEvent & event)
{
    for(Widget * w : mWidgets)
    {
        w->PropagateMouseMotion(event);
        w->HandleMouseMotion(event);
    }
}

} // namespace sgui
} // namespace lib
