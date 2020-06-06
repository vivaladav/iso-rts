#pragma once

#include "sgui/Widget.h"

#include <functional>

namespace lib
{

namespace graphic { class Image; }

namespace sgui
{

class PushButton : public Widget
{
public:
    PushButton(int wid, Widget * parent = nullptr);
    ~PushButton();

    void SetBackground(const char * file);

    void SetOnClickFunction(std::function<void()> f);

private:
    void HandleMouseButtonUp(const core::MouseButtonEvent & event) override;

    void OnRender() override;

private:
    std::function<void()> mOnClick;

    graphic::Image * mBg;
};

inline void PushButton::SetOnClickFunction(std::function<void()> f) { mOnClick = f; }

} // namespace sgui
} // namespace lib
