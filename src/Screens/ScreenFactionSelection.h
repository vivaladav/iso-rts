#pragma once

#include "Screen.h"

#include <vector>

namespace lib
{
    namespace graphic { class Image; }

    namespace sgui { class PushButton; }
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
    void InitSprites();

private:
    lib::graphic::Image * mBg = nullptr;

    PlayerFaction mFaction;
};

} // namespace game
