#pragma once

#include "sgui/Widget.h"

#include <functional>

namespace lib
{

namespace graphic
{
    class Font;
    class Renderable;
}

namespace sgui
{

class PushButton : public Widget
{
public:
    PushButton(Widget * parent = nullptr);
    ~PushButton();

    void SetBackground(const char * file);

    void SetLabel(const char * text);
    void SetLabel(const char * text, graphic::Font * font);
    void SetLabelColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void SetLabelColor(unsigned int color);

    void SetOnClickFunction(const std::function<void()> & f);

private:
    void HandlePositionChanged() override;

    void PositionLabel();

    void HandleMouseButtonUp(core::MouseButtonEvent & event) override;

    void OnRender() override;

private:
    std::function<void()> mOnClick;

    graphic::Renderable * mBg = nullptr;
    graphic::Renderable * mLabel = nullptr;
};

inline void PushButton::SetOnClickFunction(const std::function<void()> & f) { mOnClick = f; }

} // namespace sgui
} // namespace lib
