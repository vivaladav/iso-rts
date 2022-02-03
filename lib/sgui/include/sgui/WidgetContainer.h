#pragma once

#include <vector>

namespace lib
{

namespace core
{
    class KeyboardEvent;
    class MouseButtonEvent;
    class MouseMotionEvent;
}

namespace sgui
{

class Widget;

class WidgetContainer
{
public:
    virtual ~WidgetContainer();

    void ClearWidgets();

    int GetNumChildren() const;

    void MoveChildToBack(Widget * w);
    void MoveChildToFront(Widget * w);

    void ClearFocus();
    void SetFocus();
    bool HasFocus() const;

protected:
    void AddChild(Widget * w);
    void RemoveChild(Widget * w);

    void HandleChildEnableChanged(Widget * child);
    void HandleChildVisibleChanged(Widget * child);

    void PropagateMouseButtonDown(core::MouseButtonEvent & event);
    void PropagateMouseButtonUp(core::MouseButtonEvent & event);
    void PropagateMouseMotion(core::MouseMotionEvent & event);

    void PropagateKeyDown(core::KeyboardEvent & event);
    void PropagateKeyUp(core::KeyboardEvent & event);

    void PropagateRender();

    void PropagateFocus();

protected:
    std::vector<Widget *> mWidgets;

private:
    bool mFocus = false;
};

inline int WidgetContainer::GetNumChildren() const { return mWidgets.size(); }

inline bool WidgetContainer::HasFocus() const { return mFocus; }

} // namespace sgui
} // namespace lib
