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
    void SetLabelColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void SetLabelColor(unsigned int color);
    void SetLabelFont(graphic::Font * font);

    void SetOnClickFunction(const std::function<void()> & f);

protected:
    void SetCurrBg(graphic::Renderable * bg);
    void SetCurrLabel(graphic::Renderable * label);

    void HandleMouseButtonUp(core::MouseButtonEvent & event) override;

private:
    void HandlePositionChanged() override;

    void PositionLabel();

    void OnRender() override;

private:
    std::function<void()> mOnClick;

    graphic::Font * mFontLabel = nullptr;

    // actual objects created by SetBackground and SetLabel
    graphic::Renderable * mBg = nullptr;
    graphic::Renderable * mLabel = nullptr;

    // pointers to the current active objects
    graphic::Renderable * mCurrBg = nullptr;
    graphic::Renderable * mCurrLabel = nullptr;
};

inline void PushButton::SetLabelFont(graphic::Font * font) { mFontLabel = font; }

inline void PushButton::SetOnClickFunction(const std::function<void()> & f) { mOnClick = f; }

} // namespace sgui
} // namespace lib
