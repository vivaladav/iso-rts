#pragma once

#include "GameButton.h"

namespace game
{

class ButtonDialogContinue : public GameButton
{
public:
    ButtonDialogContinue(lib::sgui::Widget * parent = nullptr);
};

} // namespace game
