#pragma once

#include "Screen.h"

namespace game
{

class ScreenNewGame : public Screen
{
public:
    ScreenNewGame(Game * game);
    ~ScreenNewGame();

    void Update(float update) override;
    void Render() override;
};

} // namespace game
