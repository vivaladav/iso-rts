#pragma once

#include <core/event/KeyboardEventListener.h>
#include <core/event/MouseEventListener.h>

namespace game
{

class Game;
class SharedScreenListener;

class Screen : public lib::core::KeyboardEventListener , public lib::core::MouseEventListener
{
public:
    Screen(Game * game);
    ~Screen();

    Game * GetGame() const;

    virtual void Update() = 0;

    virtual void Render() = 0;

private:
    Game * mGame = nullptr;

    SharedScreenListener * mSharedListener = nullptr;
};

inline Game * Screen::GetGame() const { return mGame; }

} // namespace game
