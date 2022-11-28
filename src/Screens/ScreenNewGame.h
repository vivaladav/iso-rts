#pragma once

#include "Screen.h"

#include <vector>

namespace sgl
{
    namespace sgui
    {
        class Label;
        class PushButton;
    }
}

namespace game
{

class MapPreview;

enum Difficulty : unsigned int;

class ScreenNewGame : public Screen
{
public:
    ScreenNewGame(Game * game);
    ~ScreenNewGame();

    void Update(float update) override;
    void Render() override;

private:
    sgl::sgui::PushButton * mButtonStart = nullptr;

    Game * mGame = nullptr;

    int mCpuPlayers = 1;
    Difficulty mDiff;
};

} // namespace game
