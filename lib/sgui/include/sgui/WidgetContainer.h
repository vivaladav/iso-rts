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

protected:
    std::vector<Widget *> mWidgets;
};

inline int WidgetContainer::GetNumChildren() const { return mWidgets.size(); }

} // namespace sgui
} // namespace lib
