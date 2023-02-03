#pragma once

#include "GameButton.h"

namespace game
{

class ButtonDialogContinue : public GameButton
{
public:
    ButtonDialogContinue(sgl::sgui::Widget * parent = nullptr);

private:
    void HandleMouseOver() override;
    void HandleButtonDown() override;
};

} // namespace game
