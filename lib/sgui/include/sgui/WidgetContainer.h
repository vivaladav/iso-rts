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

    void OnChildEnableChanged(Widget * child);
    void OnChildVisibleChanged(Widget * child);

    void PropagateMouseButtonDown(const core::MouseButtonEvent & event);
    void PropagateMouseButtonUp(const core::MouseButtonEvent & event);
    void PropagateMouseMotion(const core::MouseMotionEvent & event);

    void PropagateRender();

protected:
    std::vector<Widget *> mWidgets;
};

inline int WidgetContainer::GetNumChildren() const { return mWidgets.size(); }

} // namespace sgui
} // namespace lib
