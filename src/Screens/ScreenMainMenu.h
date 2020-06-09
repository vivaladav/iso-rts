#pragma once

#include "Screen.h"

namespace lib
{
    namespace sgui
    {
        class PushButton;
        class Stage;
    }
}

namespace game
{

class ScreenMainMenu : public Screen
{
public:
    ScreenMainMenu(Game * game);
    ~ScreenMainMenu();

    void Update() override;
    void Render() override;

private:
    lib::sgui::Stage * mStage = nullptr;
    lib::sgui::PushButton * mButtonExit = nullptr;
    lib::sgui::PushButton * mButtonNew = nullptr;
};

} // namespace game
