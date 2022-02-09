#pragma once

#include <sgl/utilities/State.h>

namespace game
{

class Game;
class Screen;

class BaseGameState : public sgl::utilities::State
{
public:
    BaseGameState(unsigned int stateId, Game * game);
    ~BaseGameState();

    void Update(float delta);

    void Render();

protected:
    Game * mGame = nullptr;

    Screen * mScreen = nullptr;
};

inline BaseGameState::BaseGameState(unsigned int screenId, Game * game)
    : sgl::utilities::State(screenId)
    , mGame(game)
{
}

} // namespace game
