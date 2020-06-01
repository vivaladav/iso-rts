#pragma once

#include "Screen.h"

namespace lib { namespace sgui { class PushButton; } }

namespace game
{

class ScreenMainMenu : public Screen
{
public:
    ScreenMainMenu(Game * game);
    ~ScreenMainMenu();

    void Update() override;
    void Render() override;

protected:
    void OnActive() override;

private:
    lib::sgui::PushButton * mButtonNew = nullptr;
};

} // namespace game
