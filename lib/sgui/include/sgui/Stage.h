#pragma once

#include "core/event/KeyboardEventListener.h"
#include "core/event/MouseEventListener.h"
#include "sgui/WidgetContainer.h"

namespace lib
{

namespace graphic { class Font; }

namespace sgui
{

class Widget;

class Stage : public WidgetContainer, public core::KeyboardEventListener, public core::MouseEventListener
{
public:
    static Stage * Create();
    static Stage * Instance();
    static void Destroy();

    // temporary code
    graphic::Font * GetDefaultFont();
    void SetDefaultFont(graphic::Font * font);

    void Render();

private:
    Stage() = default;
    ~Stage() = default;

    void OnMouseButtonDown(const core::MouseButtonEvent & event) override;
    void OnMouseButtonUp(const core::MouseButtonEvent & event) override;
    void OnMouseMotion(const core::MouseMotionEvent & event) override;

private:
    static Stage * mInstance;

    graphic::Font * mDefaultFont = nullptr;

    // access private methods to notify changes
    friend class Widget;
};

inline Stage * Stage::Instance() { return mInstance; }

inline graphic::Font * Stage::GetDefaultFont() { return mDefaultFont; }
inline void Stage::SetDefaultFont(graphic::Font * font) { mDefaultFont = font; }

} // namespace sgui
} // namespace lib
