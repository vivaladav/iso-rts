#pragma once

#include <vector>

namespace lib
{

namespace core
{
    class MouseButtonEvent;
    class MouseMotionEvent;
}

namespace sgui
{

class Stage;

class Widget
{
public:
    Widget(int wid, Widget * parent = nullptr);
    virtual ~Widget();

    int GetWidgetId() const;

    Widget * GetParent() const;

    void SetEnabled(bool val);
    bool IsEnabled() const;

    void SetVisible(bool val);
    bool IsVisible() const;
    int GetX() const;
    int GetY() const;
    void SetPosition(int x, int y);
    void SetX(int x);
    void SetY(int y);

    int GetWidth() const;
    int GetHeight() const;

protected:
    void SetSize(int w, int h);

    void OnChildEnableChanged(Widget * child);
    void OnChildVisibleChanged(Widget * child);

private:
    void AddChild(Widget * w);
    void RemoveChild(Widget * w);

    virtual void HandleMouseButtonDown(const core::MouseButtonEvent & event);
    void PropagateMouseButtonDown(const core::MouseButtonEvent & event);
    virtual void HandleMouseButtonUp(const core::MouseButtonEvent & event);
    void PropagateMouseButtonUp(const core::MouseButtonEvent & event);
    virtual void HandleMouseMotion(const core::MouseMotionEvent & event);
    void PropagateMouseMotion(const core::MouseMotionEvent & event);

    virtual void OnRender();
    void PropagateRender();

private:
    std::vector<Widget *> mWidgets;

    Stage * mStage = nullptr;
    Widget * mParent = nullptr;

    int mId = -1;

    int mScreenX = 0;
    int mScreenY = 0;

    int mWidth = 0;
    int mHeight = 0;

    bool mEnabled = true;
    bool mVisible = true;

    // access private methods for events and rendering
    friend class Stage;
};

inline int Widget::GetWidgetId() const { return mId; }

inline Widget * Widget::GetParent() const { return mParent; }

inline bool Widget::IsEnabled() const { return mEnabled; }

inline bool Widget::IsVisible() const { return mVisible; }

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
