#pragma once

#include <sgl/sgui/AbstractButton.h>

namespace sgl { namespace graphic { class Image; } }

namespace game
{

class SettingsCheckbox : public sgl::sgui::AbstractButton
{
public:
    SettingsCheckbox(sgl::sgui::Widget * parent);

private:
    void HandleButtonDown() override;

    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override;
    void UpdateGraphics(sgl::sgui::AbstractButton::VisualState state);

    void HandlePositionChanged() override;
    void UpdatePositions();

private:
    sgl::graphic::Image * mBg = nullptr;
};

} // namespace game