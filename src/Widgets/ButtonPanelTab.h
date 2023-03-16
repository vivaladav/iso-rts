#pragma once

#include <sgl/sgui/AbstractButton.h>


namespace sgl
{
    namespace graphic { class Text; }
    namespace sgui { class Image; }
}

namespace game
{

class ButtonPanelTab : public sgl::sgui::AbstractButton
{
public:
    ButtonPanelTab(const char * text, sgl::sgui::Widget * parent);

private:
    void HandleMouseOver() override;

    void HandleButtonDown() override;

    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override;

    void HandlePositionChanged() override;

    void UpdateGraphics(sgl::sgui::AbstractButton::VisualState state);

    void UpdatePositions();

private:
    sgl::graphic::Text * mLabel = nullptr;

    sgl::sgui::Image * mBar = nullptr;
};

} // namespace game
