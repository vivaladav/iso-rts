#include "sgui/Widget.h"

#include "core/event/MouseMotionEvent.h"
#include "sgui/Stage.h"

#include <algorithm>
#include <iostream>

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

        mScreenX = parent->GetScreenX();
        mScreenY = parent->GetScreenY();
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

void Widget::HandleMouseOver()
{
    std::cout << "Widget::HandleMouseOver - ID: " << mId << std::endl;
}

void Widget::HandleMouseOut()
{
    std::cout << "Widget::HandleMouseOut - ID: " << mId << std::endl;
}

void Widget::OnChildEnableChanged(Widget * /*child*/)
{
    // TODO - handle enable changed in child
}

void Widget::OnChildVisibleChanged(Widget * /*child*/)
{
    // TODO - handle visible changed in child
}

void Widget::SetPosition(int x, int y)
{
    const int dx = x - mRelX;
    const int dy = y - mRelY;

    mRelX = x;
    mRelY = y;

    mScreenX += dx;
    mScreenY += dy;

    PropagateParentPositionChanged(dx, dy);
}

void Widget::SetX(int x)
{
    const int dx = x - mRelX;

    mRelX = x;

    mScreenX += dx;

    PropagateParentPositionChanged(dx, 0);
}

void Widget::SetY(int y)
{
    const int dy = y - mRelY;

    mRelY = y;

    mScreenY += dy;

    PropagateParentPositionChanged(0, dy);
}

bool Widget::IsScreenPointInside(int x, int y)
{
    return x > mScreenX && x < (mScreenX + mWidth) && y > mScreenY && y < (mScreenY + mHeight);
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

void Widget::SetMouseOver()
{
    // mouse already over -> nothing to do
    if(mMouseOver)
        return;

    mMouseOver = true;

    HandleMouseOver();
}

void Widget::SetMouseOut()
{
    // mouse already over -> nothing to do
    if(!mMouseOver)
        return;

    mMouseOver = false;

    // propagate out to handle case of overlapping widgets
    for(Widget * w : mWidgets)
        w->SetMouseOut();

    HandleMouseOut();
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

void Widget::OnParentPositionChanged(int dx, int dy)
{
    mScreenX += dx;
    mScreenY += dy;
}

void Widget::PropagateParentPositionChanged(int dx, int dy)
{
    for(Widget * w : mWidgets)
    {
        w->OnParentPositionChanged(dx, dy);
        w->PropagateParentPositionChanged(dx, dy);
    }
}

} // namespace sgui
} // namespace lib
