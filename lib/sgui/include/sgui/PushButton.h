#pragma once

#include "sgui/Widget.h"

#include <functional>

namespace lib
{

namespace graphic
{
    class Image;
    class Text;
}

namespace sgui
{

class PushButton : public Widget
{
public:
    PushButton(int wid, Widget * parent = nullptr);
    ~PushButton();

    void SetBackground(const char * file);

    void SetLabel(const char * text);
    void SetLabelColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    void SetOnClickFunction(std::function<void()> f);

private:
    void OnPositionChanged() override;

    void PositionLabel();

    void HandleMouseButtonUp(const core::MouseButtonEvent & event) override;

    void OnRender() override;

private:
    std::function<void()> mOnClick;

    graphic::Image * mBg = nullptr;
    graphic::Text * mLabel = nullptr;
};

inline void PushButton::SetOnClickFunction(std::function<void()> f) { mOnClick = f; }

} // namespace sgui
} // namespace lib
