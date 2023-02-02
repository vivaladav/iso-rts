#pragma once

#include "GameButton.h"

namespace game
{

class ButtonMainMenu : public GameButton
{
public:
    ButtonMainMenu(const char * text, sgl::sgui::Widget * parent);

private:
    void HandleMouseOver() override;
    void HandleButtonDown() override;
};

} // namespace game
