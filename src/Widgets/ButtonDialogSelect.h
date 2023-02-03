#pragma once

#include "GameButton.h"

namespace game
{

class ButtonDialogSelect : public GameButton
{
public:
    ButtonDialogSelect(sgl::sgui::Widget * parent = nullptr);

private:
    void HandleMouseOver() override;
    void HandleButtonDown() override;
};

} // namespace game
