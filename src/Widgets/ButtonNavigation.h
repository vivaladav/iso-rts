#pragma once

#include "GameButton.h"

namespace game
{

class ButtonNavigation : public GameButton
{
public:
    ButtonNavigation(const char * text, lib::sgui::Widget * parent = nullptr);
};

} // namespace game
