#pragma once

#include <core/event/KeyboardEventListener.h>
#include <core/event/MouseEventListener.h>

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

class Stage : public core::KeyboardEventListener, public core::MouseEventListener
{
public:
    static Stage * Create();
    static Stage * Instance();
    static void Destroy();

    void Render();

private:
    Stage();
    ~Stage();

    void AddChild(Widget * w);

    void OnChildEnableChanged(Widget * child);
    void OnChildVisibleChanged(Widget * child);

    void OnMouseButtonDown(const core::MouseButtonEvent & event);
    void OnMouseButtonUp(const core::MouseButtonEvent & event);
    void OnMouseMotion(const core::MouseMotionEvent & event);

private:
    static Stage * mInstance;

    std::vector<Widget *> mWidgets;

    // add private methods to notify changes
    friend class Widget;
};

inline Stage * Stage::Instance() { return mInstance; }

} // namespace sgui
} // namespace lib
