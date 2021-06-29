#pragma once

#include "GameButton.h"

namespace game
{

class ButtonDialogSelect : public GameButton
{
public:
    ButtonDialogSelect(lib::sgui::Widget * parent = nullptr);
};

} // namespace game
