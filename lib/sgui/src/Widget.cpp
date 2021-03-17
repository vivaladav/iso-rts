#include "sgui/Widget.h"

#include "core/event/MouseButtonEvent.h"
#include "core/event/MouseMotionEvent.h"
#include "sgui/Stage.h"

namespace lib
{
namespace sgui
{

Widget::~Widget()
{
    if(mParent)
        mParent->RemoveChild(this);
    else
        mStage->RemoveChild(this);
}

void Widget::SetParent(Widget * parent)
{
    // remove from current parent
    if(mParent)
    {
        mParent->RemoveChild(this);
        mParent->UpdateSize();
        mParent = nullptr;
    }
    else if(mStage)
    {
        mStage->RemoveChild(this);
        mStage = nullptr;
    }

    // set new parent
    if(parent)
    {
        mParent = parent;
        parent->AddChild(this);
        parent->UpdateSize();

        mScreenX = mRelX + parent->GetScreenX();
        mScreenY = mRelY + parent->GetScreenY();
    }
    else
    {
        mStage = Stage::Instance();
        mStage->AddChild(this);

        mScreenX = mRelX;
        mScreenY = mRelY;
    }
}

void Widget::SetEnabled(bool val)
{
    if(val == mEnabled)
        return ;

    mEnabled = val;

    if(val)
        HandleStateEnabled();
    else
        HandleStateDisabled();

    if(mParent)
        mParent->HandleChildEnableChanged(this);
    else
        mStage->HandleChildEnableChanged(this);
}

void Widget::SetVisible(bool val)
{
    if(val == mVisible)
        return ;

    mVisible = val;

    if(mParent)
        mParent->HandleChildVisibleChanged(this);
    else
        mStage->HandleChildVisibleChanged(this);
}

void Widget::SetSize(int w, int h)
{
    mWidth = w;
    mHeight = h;

    if(mParent)
        mParent->UpdateSize();
}

void Widget::HandleMouseOver() { }

void Widget::HandleMouseOut() { }

// current behavior for Widgets is to grow according to children position and size
void Widget::UpdateSize()
{
    // TODO decide what to do with negative position
    int maxX = 0;
    int maxY = 0;

    for(Widget * w : mWidgets)
    {
        int x = w->GetX() + w->GetWidth();
        int y = w->GetY() + w->GetHeight();

        if(x > maxX)
            maxX = x;

        if(y > maxY)
            maxY = y;
    }

    // update size if bigger than current
    if(maxX > mWidth)
        mWidth = maxX;

    if(maxY > mHeight)
        mHeight = maxY;
}

void Widget::SetPosition(int x, int y)
{
    const int dx = x - mRelX;
    const int dy = y - mRelY;

    mRelX = x;
    mRelY = y;

    SetScreenPosition(mScreenX + dx, mScreenY + dy);

    PropagateParentPositionChanged(dx, dy);

    if(mParent)
        mParent->UpdateSize();
}

void Widget::SetX(int x)
{
    const int dx = x - mRelX;

    mRelX = x;

    SetScreenPosition(mScreenX + dx, mScreenY);

    PropagateParentPositionChanged(dx, 0);

    if(mParent)
        mParent->UpdateSize();
}

void Widget::SetY(int y)
{
    const int dy = y - mRelY;

    mRelY = y;

    SetScreenPosition(mScreenX, mScreenY + dy);

    PropagateParentPositionChanged(0, dy);

    if(mParent)
        mParent->UpdateSize();
}

bool Widget::IsScreenPointInside(int x, int y)
{
    return x > mScreenX && x < (mScreenX + mWidth) && y > mScreenY && y < (mScreenY + mHeight);
}

void Widget::SetScreenPosition(int x, int y)
{
    mScreenX = x;
    mScreenY = y;

    HandlePositionChanged();
}

void Widget::HandlePositionChanged() { }

void Widget::HandleStateEnabled() { }
void Widget::HandleStateDisabled() { }

void Widget::HandleMouseButtonDown(core::MouseButtonEvent &) { }
void Widget::HandleMouseButtonUp(core::MouseButtonEvent &) { }
void Widget::HandleMouseMotion(core::MouseMotionEvent &) { }

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

void Widget::OnRender() { }

void Widget::HandleParentPositionChanged(int dx, int dy)
{
    SetScreenPosition(mScreenX + dx, mScreenY + dy);
}

void Widget::PropagateParentPositionChanged(int dx, int dy)
{
    for(Widget * w : mWidgets)
    {
        w->HandleParentPositionChanged(dx, dy);
        w->PropagateParentPositionChanged(dx, dy);
    }
}

} // namespace sgui
} // namespace lib
