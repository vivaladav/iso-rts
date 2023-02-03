#pragma once

#include "GameButton.h"

namespace game
{

class ButtonDialogBack : public GameButton
{
public:
    ButtonDialogBack(sgl::sgui::Widget * parent = nullptr);

private:
    void HandleMouseOver() override;
    void HandleButtonDown() override;
};

} // namespace game
