#include "sgui/Widget.h"

#include "sgui/Stage.h"

#include <algorithm>

namespace lib
{
namespace sgui
{

Widget::Widget(int wid, Widget * parent)
    : mId(wid)
{
    if(parent)
    {
        mParent = parent;
        parent->AddChild(this);
    }
    else
    {
        mStage = Stage::Instance();
        mStage->AddChild(this);
    }
}

Widget::~Widget()
{
    for(Widget * w : mWidgets)
        delete w;
}

void Widget::SetEnabled(bool val)
{
    if(val == mEnabled)
        return ;

    mEnabled = val;

    if(mParent)
        mParent->OnChildEnableChanged(this);
    else
        mStage->OnChildEnableChanged(this);
}

void Widget::SetVisible(bool val)
{
    if(val == mVisible)
        return ;

    mVisible = val;

    if(mParent)
        mParent->OnChildVisibleChanged(this);
    else
        mStage->OnChildVisibleChanged(this);
}

/*
void Widget::RemoveChild(Widget * w)
{
    auto res = std::find(mChildren.begin(), mChildren.end(), w);

    if(mChildren.end() == res)
        return ;

    mChildren.erase(res);
}
*/

void Widget::OnChildEnableChanged(Widget * /*child*/)
{
    // TODO - handle enable changed in child
}

void Widget::OnChildVisibleChanged(Widget * /*child*/)
{
    // TODO - handle visible changed in child
}

void Widget::AddChild(Widget * w)
{
    // widget already added
    if(std::find(mWidgets.begin(), mWidgets.end(), w) != mWidgets.end())
        return ;

    mWidgets.emplace_back(w);
}

void Widget::RemoveChild(Widget * w)
{
    auto res = std::find(mWidgets.begin(), mWidgets.end(), w);

    mWidgets.erase(res);
}

void Widget::HandleMouseButtonDown(const core::MouseButtonEvent &) { }

void Widget::PropagateMouseButtonDown(const core::MouseButtonEvent & event)
{
    for(Widget * w : mWidgets)
    {
        w->PropagateMouseButtonDown(event);
        w->HandleMouseButtonDown(event);
    }
}

void Widget::HandleMouseButtonUp(const core::MouseButtonEvent &) { }

void Widget::PropagateMouseButtonUp(const core::MouseButtonEvent & event)
{
    for(Widget * w : mWidgets)
    {
        w->PropagateMouseButtonUp(event);
        w->HandleMouseButtonUp(event);
    }
}

void Widget::HandleMouseMotion(const core::MouseMotionEvent &) { }

void Widget::PropagateMouseMotion(const core::MouseMotionEvent & event)
{
    for(Widget * w : mWidgets)
    {
        w->PropagateMouseMotion(event);
        w->HandleMouseMotion(event);
    }
}

void Widget::OnRender()
{
}

void Widget::PropagateRender()
{
    for(Widget * w : mWidgets)
    {
        w->OnRender();
        w->PropagateRender();
    }
}

} // namespace sgui
} // namespace lib
