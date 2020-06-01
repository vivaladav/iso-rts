#pragma once

#include "graphic/Image.h"
#include "sgui/Widget.h"

#include <functional>

namespace lib
{
namespace sgui
{

class PushButton : public Widget
{
public:
    PushButton(int wid, Widget * parent = nullptr);
    ~PushButton();

    void SetBackground(const char * file);

    void Render() override;

    // -- mouse event --
    void OnMouseButtonUp(const core::MouseButtonEvent & event) override;

    void SetOnClickFunction(std::function<void()> f);

private:
    std::function<void()> mOnClick;

    graphic::Image * mBg;
};

inline void PushButton::SetOnClickFunction(std::function<void()> f) { mOnClick = f; }

} // namespace sgui
} // namespace lib
