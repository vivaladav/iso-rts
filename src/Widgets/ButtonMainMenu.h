#pragma once

#include "GameButton.h"

namespace game
{

class ButtonMainMenu : public GameButton
{
public:
    ButtonMainMenu(const char * text, lib::sgui::Widget * parent);
};

} // namespace game
