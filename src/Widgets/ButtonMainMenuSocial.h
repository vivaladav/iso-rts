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
    ~ButtonMainMenuSocial();

private:
    void HandlePositionChanged() override;

    void OnRender() override;

    void OnStateChanged(lib::sgui::PushButton::VisualState state) override;

    void PositionElements();

private:
    std::array<lib::graphic::Image *, NUM_VISUAL_STATES> mBackgrounds;
    std::array<unsigned int, NUM_VISUAL_STATES> mLabelsColor;

    lib::graphic::Image * mIcon = nullptr;
};

} // namespace game
