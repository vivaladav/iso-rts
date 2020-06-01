#pragma once

#include "core/event/EventListener.h"

namespace lib
{
namespace sgui
{

class Widget : public core::EventListener
{
public:
    Widget(int wid, Widget * parent = nullptr);
    virtual ~Widget();

    int GetWidgetId() const;

    Widget * GetParent() const;

    int GetX() const;
    int GetY() const;
    void SetPosition(int x, int y);
    void SetX(int x);
    void SetY(int y);

    int GetWidth() const;
    int GetHeight() const;

    virtual void Render() = 0;

protected:
    void SetSize(int w, int h);

private:
    Widget * mParent = nullptr;

    int mId = -1;

    int mScreenX = 0;
    int mScreenY = 0;

    int mWidth = 0;
    int mHeight = 0;
};

inline Widget::Widget(int wid, Widget * parent) :  mParent(parent), mId(wid){ }
inline Widget::~Widget() { }

inline int Widget::GetWidgetId() const { return mId; }

inline Widget * Widget::GetParent() const { return mParent; }

inline int Widget::GetX() const { return mScreenX; }
inline int Widget::GetY() const { return mScreenY; }

inline void Widget::SetPosition(int x, int y)
{
    mScreenX = x;
    mScreenY = y;
}
inline void Widget::SetX(int x) { mScreenX = x; }
inline void Widget::SetY(int y) { mScreenY = y; }

inline int Widget::GetWidth() const { return mWidth; }
inline int Widget::GetHeight() const { return mHeight; }
inline void Widget::SetSize(int w, int h)
{
    mWidth = w;
    mHeight = h;
}

} // namespace sgui
} // namespace lib
