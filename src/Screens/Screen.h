#pragma once

#include <core/event/EventListener.h>
#include <utilities/State.h>

namespace game
{

class Game;

class Screen : public lib::core::EventListener, public lib::utilities::State
{
public:
    Screen(unsigned int screenId, Game * game);
    virtual ~Screen();

    virtual void Update() = 0;

    virtual void Render() = 0;

protected:
    void OnActive() override;

    Game * GetGame() const;

private:
    Game * mGame;
};

inline Screen::Screen(unsigned int screenId, Game * game)
    : lib::utilities::State(screenId)
    , mGame(game)
{
}

inline Screen::~Screen() { }

inline Game * Screen::GetGame() const { return mGame; }

} // namespace game
