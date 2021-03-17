#pragma once

#include <sgui/Widget.h>

namespace lib  { namespace graphic  { class Renderable; } }

namespace game
{

class DigitsDisplay : public lib::sgui::Widget
{
public:
    DigitsDisplay(int digits, lib::sgui::Widget * parent);

    void SetValue(int val);

private:
    void OnRender() override;

    void HandlePositionChanged() override;

private:
    int mDigits = 0;
    int mValue = -1;

    lib::graphic::Renderable * mTxtZeros = nullptr;
    lib::graphic::Renderable * mTxtDigits = nullptr;
};

} // namespace game
