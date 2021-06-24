#pragma once

#include "Screen.h"

#include <vector>

namespace lib
{
    namespace sgui
    {
        class Label;
        class PushButton;
    }
}

namespace game
{

enum PlayerFaction : unsigned int;

class ScreenFactionSelection : public Screen
{
public:
    ScreenFactionSelection(Game * game);
    ~ScreenFactionSelection();

    void Update(float update) override;
    void Render() override;

private:
    PlayerFaction mFaction;
};

} // namespace game
