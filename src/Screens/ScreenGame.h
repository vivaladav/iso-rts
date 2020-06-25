#pragma once

#include "Screen.h"

namespace lib { namespace sgui { class Widget; } }

namespace game
{

class GameMap;
class IsoMap;
class Player;

class ScreenGame : public Screen
{
public:
    ScreenGame(Game * game);
    ~ScreenGame();

    void Update() override;
    void Render() override;

    void OnMouseButtonUp(lib::core::MouseButtonEvent & event) override;

private:
    GameMap * mGameMap = nullptr;
    IsoMap * mIsoMap = nullptr;
};

} // namespace game
