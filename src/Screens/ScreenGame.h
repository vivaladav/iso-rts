#pragma once

#include "GameConstants.h"
#include "Screen.h"

#include <array>
#include <vector>

namespace game
{

class CellProgressBar;
class GameMap;
class IsoLayer;
class IsoMap;
class PanelPlayer;
class Player;

struct Cell2D;

class ScreenGame : public Screen
{
public:
    ScreenGame(Game * game);
    ~ScreenGame();

    void Update(float delta) override;
    void Render() override;

    void OnMouseButtonUp(lib::core::MouseButtonEvent & event) override;

    void GameOver();

private:
    CellProgressBar * CreateProgressBar(const Cell2D & cell, float time, int playerId);
    void UpdateProgressBars(float delta);

    void ClearSelection(Player * player);

private:
    std::array<PanelPlayer *, MAX_NUM_PLAYERS> mPanelsPlayer;

    std::vector<CellProgressBar *> mProgressBars;
    std::vector<int> mProgressBarsToDelete;

    GameMap * mGameMap = nullptr;
    IsoMap * mIsoMap = nullptr;

    struct Cell2D * mPrevSel = nullptr;

    float mTimerCoins = TIME_COINS_GEN;
};

} // namespace game
