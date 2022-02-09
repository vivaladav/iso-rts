#pragma once

#include <sgl/sgui/Widget.h>

namespace game
{

class ButtonsPanel : public sgl::sgui::Widget
{
public:
    ButtonsPanel(sgl::sgui::Widget * parent);

private:
    void HandleMouseButtonUp(sgl::core::MouseButtonEvent & event) override;
};

inline ButtonsPanel::ButtonsPanel(sgl::sgui::Widget * parent)
    : sgl::sgui::Widget(parent)
{
}

} // namespace game
