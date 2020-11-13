#pragma once

#include "Screen.h"

namespace game
{

enum Difficulty : unsigned int;

class ScreenNewGame : public Screen
{
public:
    ScreenNewGame(Game * game);
    ~ScreenNewGame();

    void Update(float update) override;
    void Render() override;

private:
    int mCpuPlayers = 1;
    Difficulty mDiff;
};

} // namespace game
