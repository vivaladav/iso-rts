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

protected:
    void SetCurrBg(graphic::Renderable * bg);
    void SetCurrLabel(graphic::Renderable * label);

private:
    void HandlePositionChanged() override;

    void PositionLabel();

    void HandleMouseButtonUp(core::MouseButtonEvent & event) override;

    void OnRender() override;

private:
    std::function<void()> mOnClick;

    // actual objects created by SetBackground and SetLabel
    graphic::Renderable * mBg = nullptr;
    graphic::Renderable * mLabel = nullptr;

    // pointers to the current active objects
    graphic::Renderable * mCurrBg = nullptr;
    graphic::Renderable * mCurrLabel = nullptr;
};

inline void PushButton::SetOnClickFunction(const std::function<void()> & f) { mOnClick = f; }

} // namespace sgui
} // namespace lib
