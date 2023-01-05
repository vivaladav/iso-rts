#pragma once

#include <sgl/core/event/KeyboardEventListener.h>

namespace game
{

class Game;
class Screen;

class SharedScreenListener : public sgl::core::KeyboardEventListener
{
public:
    SharedScreenListener(Game * game, Screen * parent);

    void OnKeyUp(sgl::core::KeyboardEvent & event) override;

private:
    Screen * mScreen = nullptr;
    Game * mGame = nullptr;
};

inline SharedScreenListener::SharedScreenListener(Game * game, Screen * parent)
    : mScreen(parent)
    , mGame(game)
{
}

} // namespace game
