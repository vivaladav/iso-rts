#pragma once

#include "core/event/KeyboardEventListener.h"
#include "core/event/MouseEventListener.h"
#include "sgui/WidgetContainer.h"

namespace lib
{
namespace sgui
{

class Widget;

class Stage : public WidgetContainer, public core::KeyboardEventListener, public core::MouseEventListener
{
public:
    static Stage * Create();
    static Stage * Instance();
    static void Destroy();

    void Render();

private:
    Stage() = default;
    ~Stage() = default;

    void OnMouseButtonDown(const core::MouseButtonEvent & event) override;
    void OnMouseButtonUp(const core::MouseButtonEvent & event) override;
    void OnMouseMotion(const core::MouseMotionEvent & event) override;

private:
    static Stage * mInstance;

    // access private methods to notify changes
    friend class Widget;
};

inline Stage * Stage::Instance() { return mInstance; }

} // namespace sgui
} // namespace lib
