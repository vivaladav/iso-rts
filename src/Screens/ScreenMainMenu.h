#pragma once

#include "Screen.h"

namespace lib { namespace graphic { class Image; } }

namespace game
{

class ScreenMainMenu : public Screen
{
public:
    ScreenMainMenu(Game * game);
    ~ScreenMainMenu();

    void Update(float update) override;
    void Render() override;

private:
    void InitSprites();

private:
    lib::graphic::Image * mBg = nullptr;
};

} // namespace game
