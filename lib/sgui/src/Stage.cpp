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
    const int x = event.GetX();
    const int y = event.GetY();

    for(Widget * w : mWidgets)
    {
        if(w->IsScreenPointInside(x, y))
        {
            w->PropagateMouseButtonDown(event);
            w->HandleMouseButtonDown(event);
        }
    }
}

void Stage::OnMouseButtonUp(const core::MouseButtonEvent & event)
{
    const int x = event.GetX();
    const int y = event.GetY();

    for(Widget * w : mWidgets)
    {
        if(w->IsScreenPointInside(x, y))
        {
            w->PropagateMouseButtonUp(event);
            w->HandleMouseButtonUp(event);
        }
    }
}

void Stage::OnMouseMotion(const core::MouseMotionEvent & event)
{
    const int x = event.GetX();
    const int y = event.GetY();

    for(Widget * w : mWidgets)
    {
        if(w->IsScreenPointInside(x, y))
        {
            w->PropagateMouseMotion(event);

            w->SetMouseOver();

            w->HandleMouseMotion(event);
        }
        else
            w->SetMouseOut();
    }
}

void Stage::AddChild(Widget * w)
{
    mWidgets.emplace_back(w);
}

void Stage::RemoveChild(Widget * w)
{
    auto res = std::find(mWidgets.begin(), mWidgets.end(), w);

    mWidgets.erase(res);
}

} // namespace sgui
} // namespace lib
