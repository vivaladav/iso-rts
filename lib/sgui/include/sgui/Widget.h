#pragma once

#include "sgui/WidgetContainer.h"

namespace lib
{
namespace sgui
{

class Stage;

class Widget : public WidgetContainer
{
public:
    Widget(Widget * parent = nullptr);
    ~Widget();

    void SetWidgetId(int wid);
    int GetWidgetId() const;

    Widget * GetParent() const;
    void SetParent(Widget * parent);

    void SetEnabled(bool val);
    bool IsEnabled() const;

    void SetVisible(bool val);
    bool IsVisible() const;

    int GetX() const;
    int GetY() const;
    int GetScreenX() const;
    int GetScreenY() const;
    void SetPosition(int x, int y);
    void SetX(int x);
    void SetY(int y);

    bool IsMouseOver() const;
    bool IsScreenPointInside(int x, int y);

    int GetWidth() const;
    int GetHeight() const;

protected:
    void SetSize(int w, int h);

    virtual void HandleMouseOver();
    virtual void HandleMouseOut();

    virtual void UpdateSize();

private:
    void SetScreenPosition(int x, int y);

    virtual void OnPositionChanged();

    virtual void HandleMouseButtonDown(const core::MouseButtonEvent & event);
    virtual void HandleMouseButtonUp(const core::MouseButtonEvent & event);
    virtual void HandleMouseMotion(const core::MouseMotionEvent & event);

    void SetMouseOver();
    void SetMouseOut();

    virtual void OnRender();

    void OnParentPositionChanged(int dx, int dy);
    void PropagateParentPositionChanged(int dx, int dy);

private:
    Stage * mStage = nullptr;
    Widget * mParent = nullptr;

    int mId = -1;

    int mRelX = 0;
    int mRelY = 0;

    int mScreenX = 0;
    int mScreenY = 0;

    int mWidth = 0;
    int mHeight = 0;

    bool mEnabled = true;
    bool mVisible = true;
    bool mMouseOver = false;

    // access private methods for events and rendering
    friend class WidgetContainer;
};


inline Widget::Widget(Widget * parent) { SetParent(parent); }

inline void Widget::SetWidgetId(int wid) { mId = wid; }
inline int Widget::GetWidgetId() const { return mId; }

inline Widget * Widget::GetParent() const { return mParent; }

inline bool Widget::IsEnabled() const { return mEnabled; }

inline bool Widget::IsVisible() const { return mVisible; }

inline int Widget::GetX() const { return mRelX; }
inline int Widget::GetY() const { return mRelY; }
inline int Widget::GetScreenX() const { return mScreenX; }
inline int Widget::GetScreenY() const { return mScreenY; }

inline bool Widget::IsMouseOver() const { return mMouseOver; }

inline int Widget::GetWidth() const { return mWidth; }
inline int Widget::GetHeight() const { return mHeight; }

} // namespace sgui
} // namespace lib
