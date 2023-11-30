#pragma once

#include <sgl/sgui/ImageButton.h>

namespace game
{

class ButtonMinimap : public sgl::sgui::ImageButton
{
public:
    ButtonMinimap(sgl::sgui::Widget * parent);

private:
    void HandleMouseOver() override;
    void HandleButtonDown() override;
};

} // namespace game
