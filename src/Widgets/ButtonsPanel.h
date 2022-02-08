#pragma once

#include <sgl/sgui/Widget.h>

namespace game
{

class ButtonsPanel : public lib::sgui::Widget
{
public:
    ButtonsPanel(lib::sgui::Widget * parent);

private:
    void HandleMouseButtonUp(lib::core::MouseButtonEvent & event) override;
};

inline ButtonsPanel::ButtonsPanel(lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
{
}

} // namespace game
