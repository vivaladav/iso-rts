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
    void GameWon();

private:
    CellProgressBar * CreateProgressBar(const Cell2D & cell, float time, int playerId);
    void UpdateProgressBars(float delta);

    void ClearSelection(Player * player);

    void UpdateAI(float delta);

    void SetupCellFortify(const Cell2D & cell, Player * player);
    void SetupCellUpgrade(const Cell2D & cell, Player * player);
    void SetupNewUnit(const Cell2D & cell, Player * player);
    void SetupUnitUpgrade(const Cell2D & cell, Player * player);

private:
    std::array<PanelPlayer *, MAX_NUM_PLAYERS> mPanelsPlayer;

    std::vector<Player *> mAiPlayers;

    std::vector<CellProgressBar *> mProgressBars;
    std::vector<CellProgressBar *> mProgressBarsToDelete;

    GameMap * mGameMap = nullptr;
    IsoMap * mIsoMap = nullptr;

    struct Cell2D * mPrevSel = nullptr;

    float mTimerCoins = TIME_COINS_GEN;
    float mTimerAI = TIME_AI_MOVE;

    unsigned int mCurrPlayerAI = 0;
};

} // namespace game
