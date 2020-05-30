#pragma once

#include <utilities/State.h>

namespace game
{

class Game;
class SharedScreenListener;

class Screen : public lib::utilities::State
{
public:
    Screen(unsigned int screenId, Game * game);
    virtual ~Screen();

    Game * GetGame() const;

    virtual void Update() = 0;

    virtual void Render() = 0;

protected:
    void OnActive() override;
    void OnInactive() override;

private:
    Game * mGame = nullptr;

    SharedScreenListener * mSharedListener = nullptr;
};

inline Game * Screen::GetGame() const { return mGame; }

} // namespace game
