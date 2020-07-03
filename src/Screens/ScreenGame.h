#pragma once

#include "GameConstants.h"
#include "Screen.h"

#include <array>

namespace lib { namespace sgui { class Widget; } }

namespace game
{

class GameMap;
class IsoLayer;
class IsoMap;
class PanelPlayer;
class Player;

class ScreenGame : public Screen
{
public:
    ScreenGame(Game * game);
    ~ScreenGame();

    void Update(float delta) override;
    void Render() override;

    void OnMouseButtonUp(lib::core::MouseButtonEvent & event) override;

private:
    std::array<PanelPlayer *, MAX_NUM_PLAYERS> mPanelsPlayer;

    GameMap * mGameMap = nullptr;
    IsoMap * mIsoMap = nullptr;

    IsoLayer * mLayerFort = nullptr;

    float mTimerCoins = TIME_COINS_GEN;
};

} // namespace game
