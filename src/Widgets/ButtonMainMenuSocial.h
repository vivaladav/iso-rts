#pragma once

#include <sgui/PushButton.h>

#include <array>

namespace lib
{
    namespace graphic { class Image; }
}

namespace game
{

class ButtonMainMenuSocial : public lib::sgui::PushButton
{

public:
    ButtonMainMenuSocial(unsigned int icon, lib::sgui::Widget * parent);

private:
    void HandlePositionChanged() override;

    void OnStateChanged(lib::sgui::PushButton::VisualState state) override;

    void PositionElements();

private:
    std::array<unsigned int, NUM_VISUAL_STATES> mLabelsColor;

    lib::graphic::Image * mBody = nullptr;
    lib::graphic::Image * mIcon = nullptr;
};

} // namespace game
