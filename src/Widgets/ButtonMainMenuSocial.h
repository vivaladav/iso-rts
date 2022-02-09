#pragma once

#include <sgl/sgui/PushButton.h>

#include <array>

namespace sgl
{
    namespace graphic { class Image; }
}

namespace game
{

class ButtonMainMenuSocial : public sgl::sgui::PushButton
{

public:
    ButtonMainMenuSocial(unsigned int icon, sgl::sgui::Widget * parent);

private:
    void HandlePositionChanged() override;

    void OnStateChanged(sgl::sgui::PushButton::VisualState state) override;

    void PositionElements();

private:
    std::array<unsigned int, NUM_VISUAL_STATES> mLabelsColor;

    sgl::graphic::Image * mBody = nullptr;
    sgl::graphic::Image * mIcon = nullptr;
};

} // namespace game
