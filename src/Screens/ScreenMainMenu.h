#pragma once

#include "Screen.h"

namespace game
{

class ScreenMainMenu : public Screen
{
public:
    ScreenMainMenu(Game * game);
    ~ScreenMainMenu();

    void Update(float update) override;
    void Render() override;
};

} // namespace game
