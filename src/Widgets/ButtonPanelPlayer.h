#pragma once

#include "GameButton.h"

namespace game
{

class ButtonPanelPlayer : public GameButton
{
public:
    ButtonPanelPlayer(lib::sgui::Widget * parent);
    ButtonPanelPlayer(const char * text, lib::sgui::Widget * parent);
};

} // namespace game
