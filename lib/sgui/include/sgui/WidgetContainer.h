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

class Widget;

class WidgetContainer
{
public:
    virtual ~WidgetContainer();

    void ClearWidgets();

    int GetNumChildren() const;

protected:
    void AddChild(Widget * w);
    void RemoveChild(Widget * w);

    void HandleChildEnableChanged(Widget * child);
    void HandleChildVisibleChanged(Widget * child);

    void PropagateMouseButtonDown(core::MouseButtonEvent & event);
    void PropagateMouseButtonUp(core::MouseButtonEvent & event);
    void PropagateMouseMotion(core::MouseMotionEvent & event);

    void PropagateRender();

protected:
    std::vector<Widget *> mWidgets;
};

inline int WidgetContainer::GetNumChildren() const { return mWidgets.size(); }

} // namespace sgui
} // namespace lib
